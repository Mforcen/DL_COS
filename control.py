import serial
import argparse
import sys
import time

def serial_ports():
	if sys.platform.startswith('win'):
		ports = ['COM%s' % (i + 1) for i in range(256)]
	elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
		# this excludes your current terminal "/dev/tty"
		ports = glob.glob('/dev/tty[A-Za-z]*')
	elif sys.platform.startswith('darwin'):
		ports = glob.glob('/dev/tty.*')
	else:
		raise EnvironmentError('Unsupported platform')

	result = []
	for port in ports:
		try:
			s = serial.Serial(port)
			s.close()
			result.append(port)
		except (OSError, serial.SerialException):
			pass
	return result


parser = argparse.ArgumentParser(description='Communicate with FLogger')
parser.add_argument('-o', '--option', help='Select option:\n\tupload file\n\tdownload flash')
parser.add_argument('-f', '--file', help='File path')
parser.add_argument('-p', '--port', help='Serial port')
parser.add_argument('-rp', '--remote-path', help='The remote')
parser.add_argument('-d', '--debug', help='Redirects flow to stdout', action='store_true')
parser.add_argument('-w', '--wait', help='Waits between two chunks', action='store_true')
parser.add_argument('-a', '--address', help='Indicates the flash address to read')
parser.add_argument('-l', '--length', help='Indicates how many bytes to read')

args = parser.parse_args()

try:
	if args.port:
		port = serial.Serial(args.port, 115200, timeout=2)
	else:
		port = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)
except Exception:
	print('No serial port found, use:')
	print('[' + ', '.join(serial_ports()) + ']')
	exit()


if args.debug:
	out = print
else:
	out = port.write

if args.option == 'upload':
	if args.file is None or args.remote_path is None:
		print('File and remote path should be provided')
		exit()
	try:
		file = open(args.file, 'rb')
		file_content = file.read()
		file.close()
		del file
	except FileNotFoundError:
		print('File ' + args.file + ' not found')
	preamble = 'file upload ' + args.remote_path + ' ' + str(len(file_content))+'\n'
	out(preamble.encode('ascii'))
	time.sleep(0.1)
	if args.wait:
		for i in range(len(file_content)//64 + 1):
			start = i*64
			end = (i+1)*64
			if end > len(file_content):
				end = len(file_content)
			input('Presione enter para mandar bloque')
			out(file_content[start:end])
	else:
		out(file_content)

elif args.option == 'download':
	if args.address is None:
		print('Address should be provided in download mode')
		exit()
	addr = int(args.address)

	if args.length is None:
		print('Length should be provided in download mode')
		exit()

	length = int(args.length)

	recv = b''

	while length > 0:
		local_len = min(length, 80)
		length -= local_len
		preamble = 'flash rawread ' + str(addr) + ' ' + str(local_len) + '\n'
		preamble_len = len(preamble)
		addr += local_len
		port.write(preamble.encode('ascii'))
		print(port.read(preamble_len+1))
		recv += port.read(local_len)
		time.sleep(0.1)
	print(recv)

port.close()
