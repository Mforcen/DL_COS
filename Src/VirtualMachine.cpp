#include "VirtualMachine.h"

VirtualMachine::VirtualMachine() : FwLogger::Module("CuleVM", VM_BINID, 0, 6)
{
	//ctor
	for(int i= 0; i < 4096; ++i) m_ram[i] = 0;
	m_stackPointer = 0;
	m_stackSize = 0;
	m_programCounter = 0;
	m_returnAddr = 0;
	m_argAddr = 0;
	m_localVarAddr = 0;
	m_execBuiltin = false;
	m_enabled = false;
	m_delayEnabled = false;
	m_saveTable = false;
	m_startTableWait = false;
	m_waitTable = false;
	init_builtinFuncs();
}

void VirtualMachine::setProgram(uint8_t* program, uint32_t addr, uint32_t length)
{
	for(int i = 0; i < length; ++i)
		m_ram[m_stackSize+i+addr] = program[i]; // de momento
	m_tableAddress = &m_ram[m_stackSize+length+addr];
}

void VirtualMachine::setStackSize(uint32_t stackSize)
{
	m_stackSize = stackSize;
	m_programCounter = stackSize;
}

void VirtualMachine::enable(bool enable)
{
	m_enabled = enable;
}

bool VirtualMachine::getEnabled()
{
	return m_enabled;
}

template<typename T>
void VirtualMachine::pack(uint32_t addr, T val)
{
	for(int i = 0; i < sizeof(T); ++i)
		m_ram[addr+i] = *( ((uint8_t*)&val)+i );
}

template<typename T>
T VirtualMachine::unpack(uint32_t addr)
{
	uint32_t retval = 0;
	for(int i = 0; i < sizeof(T); ++i)
		retval |= (m_ram[addr+i] << (8*i));
	return *((T*)&retval);
}

template<typename T>
void VirtualMachine::push(T val)
{
	pack<T>(m_stackPointer, val);
	m_stackPointer+=sizeof(T);
}

template<typename T>
T VirtualMachine::pop()
{
	m_stackPointer -= sizeof(T);
	uint32_t retval = unpack<T>(m_stackPointer);
	return retval;
}

bool VirtualMachine::loop()
{
	if((!m_enabled) || m_waitTable) return false; //si no está activo o si está esperando
	if(m_delayEnabled)
	{
		if(HAL_GetTick() - m_delayStart < m_delayTime) return true;
		m_delayEnabled = false;
	}
    uint16_t startUS = getUS();
    while(static_cast<uint16_t>(getUS()-startUS) < 50000)
	{
		if(!cycle()) return true; //false cycle means exit from loop
	}
}

void VirtualMachine::reset()
{
	m_programCounter = m_stackSize;
	m_stackPointer = 0;
	m_execBuiltin = false;
}

void VirtualMachine::resumeExec()
{
	m_waitTable = false;
}

uint8_t* VirtualMachine::getPrgName()
{
	return m_prgName;
}

uint8_t* VirtualMachine::getTableAddress()
{
	return m_tableAddress;
}

bool VirtualMachine::getSaveFlag()
{
	return m_saveTable;
}

void VirtualMachine::ackSaveFlag()
{
	m_saveTable = false;
}

bool VirtualMachine::getWaitFlag()
{
	if(m_startTableWait)
	{
		m_startTableWait = false;
		return true;
	}
	return false;
}

int VirtualMachine::getNextAlarm(int now_alarm) // now alarm is in seconds
{
    return (((now_alarm/tablePeriod)+1)*tablePeriod)%86400;
}

bool VirtualMachine::cycle()
{
	/**
	  * Implementation of stack related operations
	  */
	if(m_execBuiltin)
	{
		callBuiltin();
		return false;
	}
	Opcode op = static_cast<Opcode>(m_ram[m_programCounter]);
	if(static_cast<char>(op) < 28)
	{
		uint8_t var_type = static_cast<char>(op) & 0x01;
		uint8_t arr = (static_cast<char>(op) >> 1) & 0x01;
		uint8_t op_type = (static_cast<char>(op) >> 2) & 0x07;
		if(var_type) // uint32_t
		{
			uint32_t arr_len, left_len;
			if(op_type == 0) // puro arr literal
			{
				++m_programCounter;
				if(arr)
				{
					arr_len = unpack<uint32_t>(m_programCounter); // hay que saltarse la instrucción propiamente dicha

					m_programCounter += sizeof(uint32_t);
				}
				else
					arr_len = sizeof(uint32_t);

				left_len = arr_len;

				while(left_len > 0) // escribir tantas veces como sea necesario
				{
					push<uint32_t>(unpack<uint32_t>(m_programCounter));
					left_len -= sizeof(uint32_t);
					m_programCounter += sizeof(uint32_t);
				}
				if(arr_len > 4) push<uint32_t>(arr_len); // comprobar qué pasa si no hago esto ahora
			} // entonces se lee el número del stack, y la dirección de inicio del stack
			else if(op_type & 0x01) // los loads
			{
				uint32_t addr_load = pop<uint32_t>();

				if(arr) arr_len = pop<uint32_t>();
				else arr_len = sizeof(uint32_t);
				left_len = arr_len;

				if((op_type >> 1) & 0x01) // local
					addr_load += m_localVarAddr;
				else if((op_type >> 1) & 0x02) // arg
					addr_load += m_argAddr;

				while(left_len > 0)
				{
					push<uint32_t>(unpack<uint32_t>(addr_load)); // cargo primero el último valor
					left_len -= sizeof(uint32_t);
					addr_load += sizeof(uint32_t);
				}
				if(arr_len > sizeof(uint32_t)) push<uint32_t>(arr_len);
				++m_programCounter;
			}
			else if((op_type & 0x00) == 0) // los saves
			{
				uint32_t addr_save = pop<uint32_t>();
				uint8_t addr_space = op_type >> 1 & 0x3;

				if(arr) arr_len = pop<uint32_t>();
				else arr_len = sizeof(uint32_t);

				if(addr_space == 2) addr_save += m_localVarAddr;
				else if(addr_space == 3) addr_save += m_argAddr; // en este caso habrá que guardarlos

				while(arr_len > 0)
				{
					arr_len -= sizeof(uint32_t);
					pack<uint32_t>(addr_save+arr_len, pop<uint32_t>()); // guardar en ram el valor del vector
				}

				++m_programCounter;
			}
		}
		else // uint8_t
		{
			uint32_t arr_len, left_len;
			if(op_type == 0) // puro arr literal
			{
				++m_programCounter;
				if(arr)
				{
					arr_len = unpack<uint32_t>(m_programCounter); // hay que saltarse la instrucción propiamente dicha
					m_programCounter += sizeof(uint32_t);
				}
				else
					arr_len = sizeof(uint8_t);
				left_len = arr_len;
				while(left_len > 0)
				{
					push<uint8_t>(unpack<uint8_t>(m_programCounter));
					left_len -= sizeof(uint8_t);
					m_programCounter += sizeof(uint8_t);
				}
				if(arr_len > 1) push<uint32_t>(arr_len);
			} // entonces se lee el número del stack, y la dirección de inicio del stack
			else if(op_type & 0x01) // los loads
			{
				uint32_t addr_load = pop<uint32_t>();

				if(arr) arr_len = pop<uint32_t>();
				else arr_len = sizeof(uint8_t);
				left_len = arr_len;

				if((op_type >> 1) & 0x01) // local
					addr_load += m_localVarAddr;
				else if((op_type >> 1) & 0x02) // arg
					addr_load += m_argAddr;

				while(left_len > 0)
				{
					push<uint8_t>(unpack<uint8_t>(addr_load)); // cargo primero el último valor
					left_len -= sizeof(uint8_t);
					addr_load += sizeof(uint8_t);
				}
				if(arr_len > sizeof(uint8_t)) push<uint32_t>(arr_len);
				++m_programCounter;
			}
			else if((op_type & 0x00) == 0) // los saves
			{
				uint32_t addr_save = pop<uint32_t>();
				uint8_t addr_space = op_type >> 1 & 0x3;

				if(arr) arr_len = pop<uint32_t>();
				else arr_len = sizeof(uint8_t);

				if(addr_space == 2) addr_save += m_localVarAddr;
				else if(addr_space == 3) addr_save += m_argAddr; // en este caso habrá que guardarlos

				while(arr_len > 0)
				{
					arr_len -= sizeof(uint8_t);
					pack<uint8_t>(addr_save+arr_len, pop<uint8_t>()); // guardar en ram el valor del vector
				}

				++m_programCounter;
			}
		}
	}
	/**
	  *	Implementation of arithmetic operations
	  */
	else if(op == Opcode::ADD) // coge dos valores del stack, los suma y los pone
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint32_t>(a+b);
		++m_programCounter;
	}
	else if(op == Opcode::SUB)
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint32_t>(a-b);
		++m_programCounter;
	}
	else if(op == Opcode::MUL)
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint32_t>(a*b);
		++m_programCounter;
	}
	else if(op == Opcode::DIV)
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint32_t>(a/b);
		++m_programCounter;
	}
	else if(op == Opcode::FADD) // coge dos valores del stack, los suma y los pone
	{

		float a = pop<float>();
		float b = pop<float>();
		push<float>(a+b);
		++m_programCounter;
	}
	else if(op == Opcode::FSUB)
	{
		float a = pop<float>();
		float b = pop<float>();
		push<float>(a-b);
		++m_programCounter;
	}
	else if(op == Opcode::FMUL)
	{
		float a = pop<float>();
		float b = pop<float>();
		push<float>(a*b);
		++m_programCounter;
	}
	else if(op == Opcode::FDIV)
	{
		float a = pop<float>();
		float b = pop<float>();
		push<float>(a/b);
		++m_programCounter;
	}
	else if(op == Opcode::MOD)
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint32_t>(a%b);
		++m_programCounter;
	}
	else if(op == Opcode::DEC_S)
	{
		uint32_t prev = pop<uint32_t>();
		if(prev > 0) prev-=1;
		push<uint32_t>(prev);
		++m_programCounter;
	}
	else if(op == Opcode::INC_S)
	{
		uint32_t prev = pop<uint32_t>();
		if(prev < 0xffffffff) prev+=1;
		push<uint32_t>(prev);
		++m_programCounter;
	}

	/**
	  * Comparison operations
	  */

	else if(op == Opcode::LESS)
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint8_t>(b<a);
		++m_programCounter;
	}
	else if(op == Opcode::GREATER)
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint8_t>(b>a);
		++m_programCounter;
	}
	else if(op == Opcode::NOT)
	{
		if(pop<uint8_t>()) push<uint8_t>(0);
		else push<uint8_t>(1);
		++m_programCounter;
	}
	else if(op == Opcode::EQUALS)
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint8_t>(b==a);
		++m_programCounter;
	}
	else if(op == Opcode::FLESS)
	{
		float a = pop<float>();
		float b = pop<float>();
		push<uint8_t>(b<a);
		++m_programCounter;
	}
	else if(op == Opcode::FGREATER)
	{
		float a = pop<float>();
		float b = pop<float>();
		push<uint8_t>(b>a);
		++m_programCounter;
	}
	else if(op == Opcode::FNOT)
	{
		if(pop<float>()) push<float>(0);
		else push<uint8_t>(1);
		++m_programCounter;
	}
	else if(op == Opcode::FEQUALS)
	{
		float a = pop<float>();
		float b = pop<float>();
		push<uint8_t>(b==a);
		++m_programCounter;
	}

	else if(op == Opcode::INT2FLOAT)
	{
        float value = pop<uint32_t>();
        push<float>(value);
        ++m_programCounter;
	}

	/**
	  * Bitwise operations
	  */

	else if(op == Opcode::BIT_AND)
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint32_t>(a&b);
		++m_programCounter;
	}
	else if(op == Opcode::BIT_OR)
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint32_t>(a|b);
		++m_programCounter;
	}
	else if(op == Opcode::BIT_LS)
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint32_t>(a<<b);
		++m_programCounter;
	}
	else if(op == Opcode::BIT_RS)
	{
		uint32_t a = pop<uint32_t>();
		uint32_t b = pop<uint32_t>();
		push<uint32_t>(a>>b);
		++m_programCounter;
	}
	/**
	  * Flow control operations
	  */

	else if(op == Opcode::JMP)
	{
		m_programCounter = pop<uint32_t>(); // se salta a la siguiente dirección
	}
	else if(op == Opcode::JMP_IF)
	{
		if(pop<uint8_t>())
			m_programCounter = pop<uint32_t>();
		else
		{
			pop<uint32_t>();
			++m_programCounter;
		}
	}
	else if(op == Opcode::JMP_SZ)
	{
		uint32_t stackval = pop<uint32_t>();
		if(stackval == 0)
			m_programCounter = pop<uint32_t>();
		else
		{
			push<uint32_t>(stackval);
			++m_programCounter;
		}
	}
	// en este momento en el stack hay n argumentos, después se hace un respaldo del contexto que se deja atrás y se leen dos números,
	// la cantidad de argumentos que hay (que se utiliza para echar para atrás el puntero m_argAddr) y el número de variables locales,
	// que se utiliza para avanzar m_stackPointer de forma que se quede un espacio sin usar
	else if(op == Opcode::CALL)
	{
		uint32_t retAddr = m_programCounter + 1;
		m_programCounter = pop<uint32_t>(); // lee la dirección desde el stack
		if(m_programCounter > 0xffff)
		{
			m_currBuiltin.func_addr = m_programCounter&0xffff;
			m_programCounter = retAddr;
			m_execBuiltin = true;
			prepareBuiltin();
			callBuiltin();
		}
		else
		{
			push<uint32_t>(m_returnAddr); // si es una llamada a función anidada, hay que saber dónde volver antes de cambiar nada
			m_returnAddr = retAddr;
			push<uint32_t>(m_localVarAddr);
			push<uint32_t>(m_argAddr);
			m_argAddr = m_stackPointer-(unpack<uint32_t>(m_programCounter)+12);// el primer valor de m_programCounter es el tamaño de las variables, y restar el 4 es para contrarrestar el contexto
			m_programCounter += sizeof(uint32_t);
			m_localVarAddr = m_stackPointer + 4; // ahora mismo, stackPointer vale m_argAddr del stack frame que dejamos atrás, por lo que las variables locales vienen después de eso
			m_stackPointer = m_localVarAddr + unpack<uint32_t>(m_programCounter);
			m_programCounter += sizeof(uint32_t);
		}
	}
	else if(op == Opcode::RETURN)
	{
		m_programCounter = m_returnAddr; // esto fijo
		pack<uint32_t>(m_argAddr, pop<uint32_t>()); // asumo que se devuelve una variable de 4 bytes y la pongo en la dirección de m_argAddr, que era la del primer parámetro.
		m_stackPointer = m_argAddr; // se corrige el desfase de que load arg funciona distinto de pop<uint32_t> y push<uint32_t>
		m_argAddr = unpack<uint32_t>(m_localVarAddr-4);
		m_returnAddr = unpack<uint32_t>(m_localVarAddr-12);
		m_localVarAddr = unpack<uint32_t>(m_localVarAddr-8);
	}
	else if(op == Opcode::DELAY)
	{
		m_delayEnabled = true;
		m_delayTime = pop<uint32_t>();
		m_delayStart = HAL_GetTick();
		m_programCounter++;
		return false;
	}
	else if(op == Opcode::WAIT_TABLE) //wait until the next table call
	{
		m_waitTable = true; // this flag will go down when the RTC ISR is called
		m_startTableWait = true;
		m_programCounter++;
		return false;
	}
	else if(op == Opcode::SAVE_TABLE)
	{
		m_saveTable = true;
		m_programCounter++;
	}
	else // solo se ejecuta en caso de que se esté procesando una instrucción no listada
	{
		return false;
	}
	return true;
}

void push_uint32(uint8_t* buf, uint32_t val)
{
	buf[0] = val & 0xff;
	buf[1] = (val >> 8) & 0xff;
	buf[2] = (val >> 16) & 0xff;
	buf[3] = (val >> 24) & 0xff;
}

int VirtualMachine::getConfigSize()
{
	return 0;
}

int VirtualMachine::getConfig(uint8_t* buf)
{
	return 0;
}
int VirtualMachine::setConfig(uint8_t* buf)
{
	return 0;
}

int VirtualMachine::getStatus(uint8_t* buf)
{
	buf[0] = m_enabled;
	buf[1] = m_delayEnabled;
	buf[2] = m_execBuiltin;
	buf[3] = m_waitTable;
	buf[4] = m_saveTable;
	push_uint32(buf+5, m_programCounter);
	push_uint32(buf+9, m_stackPointer);
	push_uint32(buf+13, m_stackSize);
	push_uint32(buf+17, m_returnAddr);
	push_uint32(buf+21, m_localVarAddr);
	push_uint32(buf+25, m_delayStart);
	push_uint32(buf+29, m_delayTime);
	strcpy(reinterpret_cast<char*>(buf+33), reinterpret_cast<char*>(m_prgName));

	int namelen = strlen(reinterpret_cast<char*>(m_prgName));

	buf[33+namelen] = 0;

	return namelen+34; // 33+1
}

int VirtualMachine::bin_eval(uint8_t* buf, uint8_t* outbuf)
{
	if(buf[0] == 's')
	{
		outbuf[0] = 's';
		return getStatus(outbuf+1)+1;
	}
	return 0;
}

void VirtualMachine::prepareBuiltin()
{
	BuiltinFunc currFunc = builtinFuncs[m_currBuiltin.func_addr];
	m_currBuiltin.nArgs = currFunc.nArgs;
	for(int i = 0; i < currFunc.nArgs; ++i)
	{
		if(currFunc.argTypes[i] == CHAR)
		{
            m_currBuiltin.args[i].ival = pop<uint8_t>();
		}
		else if(currFunc.argTypes[i] == INT || currFunc.argTypes[i] == UINT || currFunc.argTypes[i] == FLOAT)
		{
            m_currBuiltin.args[i].ival = pop<uint32_t>();
		}
		else if(currFunc.argTypes[i] == ARR)
		{
			/*int arr_len = pop<uint32_t>();
			m_currBuiltin.args[i].ptr_val = reinterpret_cast<intptr_t>(&m_ram[m_stackPointer-arr_len]);
			m_stackPointer -= arr_len; // values shouldn't been overwritten since the vm is blocked until the function is not finished*/
			m_currBuiltin.args[i].ptr_val = pop<intptr_t>()+reinterpret_cast<intptr_t>(m_ram);
		}
	}
}

void VirtualMachine::callBuiltin()
{
	Arg rv;
	uint32_t rc = 0;
    if(m_currBuiltin.nArgs == 0)
		rc = reinterpret_cast<uint32_t (*)(Arg*)>(builtinFuncs[m_currBuiltin.func_addr].func_ptr)(&rv);
	else if(m_currBuiltin.nArgs == 1)
		rc = reinterpret_cast<uint32_t (*)(Arg*, Arg)>(builtinFuncs[m_currBuiltin.func_addr].func_ptr)(&rv, m_currBuiltin.args[0]);
	else if(m_currBuiltin.nArgs == 2)
		rc = reinterpret_cast<uint32_t (*)(Arg*, Arg, Arg)>(builtinFuncs[m_currBuiltin.func_addr].func_ptr)(&rv, m_currBuiltin.args[0], m_currBuiltin.args[1]);
	else if(m_currBuiltin.nArgs == 3)
		rc = reinterpret_cast<uint32_t (*)(Arg*, Arg, Arg, Arg)>(builtinFuncs[m_currBuiltin.func_addr].func_ptr)(&rv, m_currBuiltin.args[0], m_currBuiltin.args[1], m_currBuiltin.args[2]);
	else if(m_currBuiltin.nArgs == 4)
		rc = reinterpret_cast<uint32_t (*)(Arg*, Arg, Arg, Arg, Arg)>(builtinFuncs[m_currBuiltin.func_addr].func_ptr)(&rv, m_currBuiltin.args[0], m_currBuiltin.args[1], m_currBuiltin.args[2], m_currBuiltin.args[3]);
	else if(m_currBuiltin.nArgs == 5)
		rc = reinterpret_cast<uint32_t (*)(Arg*, Arg, Arg, Arg, Arg, Arg)>(builtinFuncs[m_currBuiltin.func_addr].func_ptr)(&rv, m_currBuiltin.args[0], m_currBuiltin.args[1], m_currBuiltin.args[2], m_currBuiltin.args[3], m_currBuiltin.args[4]);

	if(builtinFuncs[m_currBuiltin.func_addr].retValue == CHAR)
		push<uint8_t>(rv.buf[0]);
	else if(builtinFuncs[m_currBuiltin.func_addr].retValue == INT || builtinFuncs[m_currBuiltin.func_addr].retValue == UINT || builtinFuncs[m_currBuiltin.func_addr].retValue == FLOAT)
		push<uint32_t>(rv.uival);

	if(rc == 0)
		m_execBuiltin = false;
}
