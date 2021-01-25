#ifndef SSD1306_H
#define SSD1306_H

#include "stm32hal_libs.h"
#include "Log.h"
#include "mem_structs.hpp"
#include "Module.h"

namespace FwLogger
{
	class SSD1306 : public Module
	{
		public:
			SSD1306(I2C_HandleTypeDef* hi2c, uint8_t addr, uint16_t width, uint16_t height);
			void Init();

			enum Color : uint8_t
			{
				Black = 0,
				White = 1
			};

			void setColor(Color color);
			void fill(Color color = Black);
			void updateScreen();
			void drawPixel(uint8_t x, uint8_t y, Color color);
			char writeChar(char c);
			char writeString(char* str);
			void setCursor(uint8_t x, uint8_t y);

			bool loop();
			void isr();

		protected:

		private:
			uint16_t _cursorX, _cursorY, _width, _height;
			uint8_t _addr, _buf[128*4];
			bool _isrFlag, _pendingOp;

			Color _color;
			bool _inverted, _initialized;

			I2C_HandleTypeDef* _hi2c;
			void writeCommand(uint8_t command); //blocking

			circular_buffer<32, uint16_t> _cmds;
	};
}

#endif // SSD1306_H
