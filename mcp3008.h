#ifndef mcp3008_H
#define mcp3008_H
#include <stdio.h>
#include <cassert>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"


// A Class To Manage A MCP3008 For A RP2040-Based Board (That Has Hardware SPI). 
// Can Optionally Deinit The SPI Device When It Is Destroyed
class I_MCP3008
{
protected:
	// Holds A Flag As To Whether It Should Deinit The SPI Device When Destroyed
	bool flag_spiDev_internally_managed;
	// A Struct Holding The SPI Device Info
	spi_inst_t* spiDev;
	// The Chip Select Pin
	uint8_t cs;
	// A 3-Byte Buffer Holding Bytes That Will Be Sent To The MCP3008
	uint8_t writeBuffer[3]{0x01, 0x00, 0x00};
	// A 3-Byte Buffer To Hold The Value Returned From The MCP3008;
	uint8_t readBuffer[3];
	// Not Critical To The Function Of This Library, However It May Break Code That Depends On This Value
	uint CLK_RATE;
public:
	// Primary Constructor For An Uninitialised SPI Device. It Will Initialise It. 
	// By Default When This Object is Destroyed, It Will Also Deinit The SPI Device
	// Takes A SPI Device And Initialises It (Requires A Clock Speed To Be Provided), As Well As A ChipSelect Pin
	I_MCP3008(spi_inst_t* spiDevice, const uint CLK_SPEED, const uint8_t cs, const uint8_t miso, const uint8_t mosi, const uint8_t clk)
	{
		// Save All Pins That Will Be Used
		this->cs = cs;
		// Save spiDevice Pointer
		this->spiDev = spiDevice;

		// Reinitialize The SPI Device With The Desired Paramaters, And Save The Resulting Clock Rate
		spi_deinit(this->spiDev);
		this->CLK_RATE = spi_init(this->spiDev, CLK_SPEED
		);
		spi_set_format(this->spiDev, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);


		// Set The Functions For SPI-Related Pins
		gpio_set_function(clk,  GPIO_FUNC_SPI);
		gpio_set_function(mosi, GPIO_FUNC_SPI);
		gpio_set_function(miso, GPIO_FUNC_SPI);
		gpio_set_function(cs,   GPIO_FUNC_SPI);

		// Ensure Chip Select Line Is Configured Correctly
		gpio_init(cs);
		gpio_set_dir(cs, GPIO_OUT);
		gpio_put(cs, 1);

		// Set A Flag Which Ensures The SPI Bus Will Be Deinitialised Upon Object Destruction. Mainly Used For When A Preconfigured SPI Bus Is Provided
		this->flag_spiDev_internally_managed = true;
	}

	// A Secondary Constructor For An Already Initialised SPI Device
	// By Default When This Object Is Destroyed, It Will NOT Deinit The SPI Device
	// Takes A SPI Device And A Chip Select Pin
	I_MCP3008(spi_inst_t* spiDevice, const uint8_t cs) 
	{
		// Save Chip Select Pin For Later Read/Write Operations
		this->cs = cs;
		// Save spiDevice Pointer For Later Read/Write Operations
		this->spiDev = spiDevice;

		// Not Used Internally. Could Be Used Externally Through Getter
		this->CLK_RATE = spi_get_baudrate(this->spiDev);

		// Ensure Chip Select Line Is Configured Correctly
		gpio_init(cs);
		gpio_set_dir(cs, GPIO_OUT);
		gpio_put(cs, 1);

		this->flag_spiDev_internally_managed = false;
	}

	// Deinits The SPI Device If SPI Device Managment Flag Is High
	~I_MCP3008() 
	{
		if (this->flag_spiDev_internally_managed)
			spi_deinit(this->spiDev);
	}
	
	// Read Analog Value From Specified Channel
	// Function Is Alias To SPIxADC(channel, false) With Error Checking
	// Valid Values Are [0-7]
	uint16_t readADC(uint8_t channel) 
	{
		// As the channel is an unsigned integer, ne need to check if value is less than 0
		// Asserts that the channel value provided is within [0, 7]
		assert(channel < 8);
		return SPIxADC(channel, false);
	}
	// Read Analog Value From The Difference Between 2 Channels
	// Please Refer To Differential Enum For Selecting Channel
	// Function is Alias To SPIxADC(differential, true) Wirh Error Checking
	// Valid Values Are [0-7]
	uint16_t readADCDiff(uint8_t differential)
	{
		assert(differential < 8);
		return SPIxADC(differential, true);
	}


	// Due To How pico-sdk Handles SPI Communication, A Specified Clock Rate May Not Be Exactly Matched
	// This Function Returns Value Of spi_get_baudrate(spiDevice) When Object Was Initialised
	// Returns The Clock Rate Of The SPI Device At Time Of Initialisation Of Object
	uint getClockRate() { return this->CLK_RATE; }
	void updateClockRate() { this->CLK_RATE = spi_get_baudrate(this->spiDev); }
	
	// Be Careful In Using This As It May Lead To An DeInitialized SPI, Which Should Have Been Kept Initialized. Undefined Behaviour May Occur Due To Later Dependencies Of The SPI Device
	// If SPIinternallyManaged Flag Is High When The Object Is Deleted/Destroyed, It Will deinit The SPI Device, Otherwise It Will Leave It Alone
	// Sets The SPIManagment Flag
	void setFlag_SPIinternallyManaged(bool val) { this->flag_spiDev_internally_managed = val; }
	// Returns The Current State Of The SPIinternallyManaged Flag, Which Controls Whether The SPI Device Should Be Deinited Upon Object Destruction
	bool getFlag_SPIinternallyManaged() { return this->flag_spiDev_internally_managed; }

	// Holds Combinations Of Different Differential Configurations For readADCDiff(differential)
	// Below Is How The MCP3008 Interprets Differnet Differentials
	// A - B = _A_SUB_B	
	// Where A,B Are Channels
	enum class DIFFERENTIALS : uint8_t
	{
		// Channel 0 - Channel 1
		_0_SUB_1 = 0,
		// Channel 1 - Channel 0
		_1_SUB_0,
		// Channel 2 - Channel 3
		_2_SUB_3,
		// Channel 3 - Channel 2
		_3_SUB_2,
		// Channel 4 - Channel 5
		_4_SUB_5,
		// Channel 5 - Channel 4
		_5_SUB_4,
		// Channel 6 - Channel 7
		_6_SUB_7,
		// Channel 7 - Channel 6
		_7_SUB_6,
	};

	// THERE ARE NO GUARDS AGAINST MISUSE! 
	// Reads Data From The Chip 'Directly'
	// Takes A uint8_t and a bool, representing the Channel/Differential Selection And A bool to determine how to interpret the previous paramater. (false => channel, true => differential)
	// Refer To I_MCP3008::DIFFERENTIALS As To How Differentials Are Applied
	uint16_t SPIxADC(uint8_t channel, bool differential)
	{
		writeBuffer[0] = 0x01;
		writeBuffer[1] = ( differential << 7) | ((8 + channel) << 4);
		writeBuffer[2] = 0;

		gpio_put(cs, 0);
		spi_write_read_blocking(this->spiDev, writeBuffer, readBuffer, sizeof(writeBuffer));
		gpio_put(cs, 1);
		
		return (readBuffer[1] << 8) | readBuffer[2];;
	}
};
#endif