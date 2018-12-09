#define MOVE 0
#define ADD 1
#define SUB 2
#define BRA 3
#define CMP 4
#define BEQ 5
#define BNE 6
#define EXG 7
#define STOP 15

void main(void)
{
unsigned short int PC = 0 ;/* program counter */
unsigned short int DO = 0; /* data register */
unsigned short int A0 = 0;/* address register */
unsigned short int CCR = 0;/* condition code register */
unsigned short int MAR;/* memory address register */
unsigned short int MBR;/* memory buffer register */
unsigned short int IR;/* instruction register */
unsigned short int operand;/* the 8-bit operand from the IR */
unsigned short int source;/* source operand */
unsigned short int destination;
unsigned short int opcode;
unsigned short int amode;
unsigned short int i =0;
unsigned short int direction;
unsigned short int memory[256];
unsigned short int run = 1;

while (run)
{
printf("Please enter your instruction code followed by ',' then operand (all in hexadecimal)");
scanf(" %d,", &memory[i]);
scanf(" %d", &memory[i+1]);
i+=2;
MAR = PC;
PC = PC + 1;
MBR = memory[MAR]; //get the opcode
IR = MBR;
opcode = IR;
MAR = PC;
PC = PC + 1;
MBR = memory[MAR] ;//get the operand
IR = MBR;
operand = IR;
amode = opcode & 0x03;
direction = (opcode & 0x04) >>2; //divides it by 4
opcode = opcode >> 4;// divides by 8

/// this is the addressing mode bits which are used to identify the type of the mode used
switch (amode)
{
case 0:{source = memory[operand];break;} /* absolute/ indirect */
case 1:{source = operand; break; }/* literal/ immediate "*/
case 2:{source = memory[A0 + operand];break;} /* indexed */
case 3:{source = memory[PC + operand];break;} /* PC relative */
}

/// see which instruction we are using

switch (opcode)
{
case MOVE: {if (direction == 0) destination = DO;   ///1
else DO = source;
if (DO == 0) CCR = 1; else CCR = 0; /* update CCR */
break;
}

case ADD: {if (direction == 0)                      ///2

{ destination = DO + source;
if (destination == 0) CCR = 1; else CCR = 0;
}
else
{ DO = DO + source;
if (DO == 0 ) CCR = 1; else CCR = 0;
}
break;
}


case SUB: {if (direction == 0)                  ///3

{ destination = DO - source;
if (destination == 0) CCR = 1; else CCR = 0;
}
else
{ DO = DO - source;
if (DO == 0 ) CCR = 1; else CCR = 0;
}
break;
}

case BRA: { if (amode ==0 ) PC = operand;                   ///4

if (amode ==1 ) PC = PC + operand; break;
}

case CMP: { MBR = DO - source;
if (MBR == 0) CCR = 1;
else CCR = 0; break;
}

case BEQ: {if (CCR == 1)                    ///5 jump to another instruction but if the zero flag is one

{ if (amode ==0 ) PC = operand;
if (amode ==1 ) PC = PC + operand;
} break;
}

case BNE: {if (CCR != 1)                    ///6

{ if (amode ==0 ) PC = operand;
if (amode == 1) PC = PC + operand;
} break;
}

case EXG: {MBR = DO; DO = A0; A0 = MBR; break;                          ///7 exchange values of d0 and A0

}

case STOP: {run = 0; break;}

}
/* save result in memory if register to memory */
if (direction == 0)
switch (amode)
{
case 0: { memory[operand] = destination ;break; /* absolut e */}
case 1:{break; }/* literal */
case 2:{memory[A0+operand] = destination ; break; }/* indexed */
case 3 :{memory[PC+ operand] = destination ; break;} /* PC relative */
}
printf("DO= %d, PC= %d, CCR= %d\n",DO,PC,CCR);
}
printf ("BYE BYE");
}

