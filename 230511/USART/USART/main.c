#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void UART0_init(void);
void UART0_transmit(char data);	     //UART가 2개인데 그 중에서 우리는 0번을 쓸 꺼라서 0을 붙인다.
unsigned char UART0_receive(void);
volatile uint8_t isIdle;
//초기화 하는 함수                    //데이터 시트 191p
void UART0_init(void)
{
	UBRR0H = 0x00;		            // 2개의 8비트 레지스터를 조합한 16비트 레지스터, UART0을 사용하기 위한 레지스터
	UBRR0L = 207;   	     // 2배속 모드로 설정, 왜냐하면 보율을 9600으로 잡았기 떄문에
	
	UCSR0A |= (1<<U2X0);  	     //비동기, 8비트, 패리티없음, 1비트 정지 (stop 비트는 1비트로 보내준다.), 시리얼 통신의 상태를 확인하고 통신 과정을 제어하기 위한 레지스터 중 하나,
	// 2배속 모드 설정
	
	//UCSR0C |= 0x06;            // 데이터 형식 및 통신 방법을 결정하기 위해 사용
	UCSR0B |= (1<<RXEN0);        // 수신완료 인터럽트 발생을 허용
	UCSR0B |= (1<<TXEN0);        // 송신 완료 인터럽트 발생을 허용
}

void UART0_transmit(char data)		// UDRE0 비트 사용
{
	while ( !(UCSR0A & (1<<UDRE0))); //송신 가능 대기, 송신 버퍼가 비어 데이터를 받을 준비가 되어 있는지
	
	UDR0 = data;   					 //데이터 전송
	
}
unsigned char UART0_receive(void)
{
	while ( !(UCSR0A & (1<<RXC0)));  // 버퍼에 읽지 않는 문자가 있으면 1, 버퍼가 비어있으면 0.
	return UDR0;  					//실질적으로 데이터에 드나드는 것
	
	
}


int main(void)
{
	DDRA = 0xff;
	PORTA = 0x00;
	uint8_t data;
	
	UART0_init();
	while (1)
	{	
		UART0_transmit(UART0_receive());   //보냄과 동시에 받는다
		data = UART0_receive();
		UART0_transmit(data);
		static int led = 0;
		
		
		
		if (data =='a')
		{
			PORTA = 0xff;
		}
		
		if (data == 'b')
		{
			PORTA = 0x00;
		}	
		
		
		if(data == 'c')
		{
			//UDR0 = UART0_receive();
			//if(UDR0 !='c') break;   //보냄과 동시에 받는다
			PORTA = (1 << led);
			led++;
			
			if (led == 8)
			{
				led = 0;
			}
			
		}
		
		
	
	}
}
