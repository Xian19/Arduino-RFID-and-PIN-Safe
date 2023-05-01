#include <Keypad.h>


#define redPin A3
#define greenPin A4
#define Relay 1

int count = 0;
int out = 0;
String passCode ="1123A";
String currentCode = "1";
String tempString;

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 




// Control an 8x8 LED display with 2 x 74HC595 shift registers
// Using only 3 pins from the Arduino

// while condition variable
int whileVar = 0;

//Pin connected to Pin 12 of 74HC595 (Latch)
int latchPin = 11;

//Pin connected to Pin 11 of 74HC595 (Clock)
int clockPin = 12;

//Pin connected to Pin 14 of 74HC595 (Data)
int dataPin = 10;

uint8_t led[8];
uint8_t letters[672];
uint8_t currentdisplay[8];

long counter1 = 0;

// Current Character in the charMessage Array
int charMessageCurrent = 0;

// Current line in Letter
int lineLetter = 672;

//currentChar is the current character in the charMessage that is being chekced
char currentChar =32;

// Used to store the message instead of displayMessage
char charMessage[40];

// scrollMessage is array holding the message that will be displayed
uint8_t scrollMessage[480];

// Serial read Byte
int incomingByte = 0;

void setup() {

//Serial.begin(9600);
 
// Seed Random Generator with noise from analog pin 0 
randomSeed(analogRead(0));
 
//set pins to output
pinMode(latchPin, OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(dataPin, OUTPUT);
pinMode(redPin, OUTPUT);
pinMode(greenPin, OUTPUT);
pinMode(Relay, OUTPUT);

// Symbol [ ] space
letters[0] =  B00000000;
letters[1] =  B00000000;
letters[2] =  B00000000;
letters[3] =  B00000000;
letters[4] =  B00000000;
letters[5] =  B00000000;
letters[6] =  B00000000;
letters[7] =  B00000000;

// Symbol !
letters[8]  =  B00000000;
letters[9]  =  B00000000;
letters[10] =  B00000000;
letters[11] =  B00000000;
letters[12] =  B11110011;
letters[13] =  B00000000;
letters[14] =  B00000000;
letters[15] =  B00000000;

// Symbol "
letters[16] =  B00000000;
letters[17] =  B00000000;
letters[18] =  B00000000;
letters[19] =  B11100000;
letters[20] =  B00000000;
letters[21] =  B11100000;
letters[22] =  B00000000;
letters[23] =  B00000000;

// Symbol #
letters[24] =  B00000000;
letters[25] =  B00100100;
letters[26] =  B11111111;
letters[27] =  B00100100;
letters[28] =  B00100100;
letters[29] =  B00100100;
letters[30] =  B11111111;
letters[31] =  B00100100;

// Symbol $
letters[32] =  B00000000;
letters[33] =  B01001110;
letters[34] =  B10010001;
letters[35] =  B10010001;
letters[36] =  B11111111;
letters[37] =  B10010001;
letters[38] =  B10010001;
letters[39] =  B01100110;

// Symbol %
letters[40] =  B00000000;
letters[41] =  B10000111;
letters[42] =  B01000101;
letters[43] =  B00110111;
letters[44] =  B00011000;
letters[45] =  B11100100;
letters[46] =  B10000010;
letters[47] =  B11100001;

// Symbol &
letters[48] =  B00000000;
letters[49] =  B01100000;
letters[50] =  B10010101;
letters[51] =  B10010011;
letters[52] =  B10010101;
letters[53] =  B10011001;
letters[54] =  B01011001;
letters[55] =  B00111110;

// Symbol '
letters[56] =  B00000000;
letters[57] =  B00000000;
letters[58] =  B00000000;
letters[59] =  B11100000;
letters[60] =  B00000000;
letters[61] =  B00000000;
letters[62] =  B00000000;
letters[63] =  B00000000;

// Symbol (
letters[64] =  B00000000;
letters[65] =  B00000000;
letters[66] =  B10000001;
letters[67] =  B01000010;
letters[68] =  B00100100;
letters[69] =  B00011000;
letters[70] =  B00000000;
letters[71] =  B00000000;

// Symbol )
letters[72] =  B00000000;
letters[73] =  B00000000;
letters[74] =  B00011000;
letters[75] =  B00100100;
letters[76] =  B01000010;
letters[77] =  B10000001;
letters[78] =  B00000000;
letters[79] =  B00000000;

// Symbol *
letters[80] =  B00000000;
letters[81] =  B10010010;
letters[82] =  B01010100;
letters[83] =  B00111000;
letters[84] =  B11111111;
letters[85] =  B00111000;
letters[86] =  B01010100;
letters[87] =  B10010010;

// Symbol +
letters[88] =  B00000000;
letters[89] =  B00010000;
letters[90] =  B00010000;
letters[91] =  B00010000;
letters[92] =  B11111111;
letters[93] =  B00010000;
letters[94] =  B00010000;
letters[95] =  B00010000;

// Symbol ,
letters[96]  =  B00000000;
letters[97]  =  B00000000;
letters[98]  =  B00000000;
letters[99]  =  B00000110;
letters[100] =  B00000001;
letters[101] =  B00000000;
letters[102] =  B00000000;
letters[103] =  B00000000;

// Symbol -
letters[104] =  B00000000;
letters[105] =  B00000000;
letters[106] =  B00000000;
letters[107] =  B00010000;
letters[108] =  B00010000;
letters[109] =  B00010000;
letters[110] =  B00000000;
letters[111] =  B00000000;

// Symbol .
letters[112] =  B00000000;
letters[113] =  B00000000;
letters[114] =  B00000000;
letters[115] =  B00000011;
letters[116] =  B00000011;
letters[117] =  B00000000;
letters[118] =  B00000000;
letters[119] =  B00000000;

// Symbol /
letters[120] =  B00000000;
letters[121] =  B10000000;
letters[122] =  B01000000;
letters[123] =  B00100000;
letters[123] =  B00011000;
letters[125] =  B00000100;
letters[126] =  B00000010;
letters[127] =  B00000001;

// Number 0 - zero
letters[128] =  B00000000;
letters[129] =  B00111100;
letters[130] =  B01000010;
letters[131] =  B10100001;
letters[132] =  B10010001;
letters[133] =  B10001001;
letters[134] =  B01000010;
letters[135] =  B00111100;

// Number 1
letters[136] =  B00000000;
letters[137] =  B00000000;
letters[138] =  B00000001;
letters[139] =  B11111111;
letters[140] =  B01000001;
letters[141] =  B00100001;
letters[142] =  B00000000;
letters[143] =  B00000000;

// Number 2
letters[144] =  B00000000;
letters[145] =  B01100001;
letters[146] =  B10010001;
letters[147] =  B10001001;
letters[148] =  B10001001;
letters[149] =  B10000101;
letters[150] =  B10000011;
letters[152] =  B01100001;

// Number 3
letters[152] =  B00000000;
letters[153] =  B01111110;
letters[154] =  B10011001;
letters[155] =  B10011001;
letters[156] =  B10011001;
letters[157] =  B10000001;
letters[158] =  B10000001;
letters[159] =  B01000110;

// Number 4
letters[160] =  B00000000;
letters[161] =  B00000100;
letters[162] =  B11111111;
letters[163] =  B01000100;
letters[164] =  B00100100;
letters[165] =  B00010100;
letters[166] =  B00001100;
letters[167] =  B00000100;

// Number 5
letters[168] =  B00000000;
letters[169] =  B10001110;
letters[170] =  B10010001;
letters[171] =  B10010001;
letters[172] =  B10010001;
letters[173] =  B10010001;
letters[174] =  B10010001;
letters[175] =  B11100010;

// Number 6
letters[176] =  B00000000;
letters[177] =  B01001110;
letters[178] =  B10010001;
letters[179] =  B10010001;
letters[180] =  B10010001;
letters[181] =  B10010001;
letters[182] =  B10010001;
letters[183] =  B01111110;

// Number 7
letters[184] =  B00000000;
letters[185] =  B11100000;
letters[186] =  B10010000;
letters[187] =  B10001000;
letters[188] =  B10000111;
letters[189] =  B00000000;
letters[190] =  B00000000;
letters[191] =  B00000000;

// Number 8
letters[192] =  B00000000;
letters[193] =  B01100110;
letters[194] =  B10011001;
letters[195] =  B10011001;
letters[196] =  B10011001;
letters[197] =  B10011001;
letters[198] =  B10011001;
letters[199] =  B01100110;

// Number 9
letters[200] =  B00000000;
letters[201] =  B01111110;
letters[202] =  B10001001;
letters[203] =  B10001001;
letters[204] =  B10001001;
letters[205] =  B10001001;
letters[206] =  B10001001;
letters[207] =  B01110010;

// Symbol :
letters[208] =  B00000000;
letters[209] =  B00000000;
letters[210] =  B00000000;
letters[211] =  B00000000;
letters[212] =  B01100110;
letters[213] =  B00000000;
letters[214] =  B00000000;
letters[215] =  B00000000;

// Symbol ;
letters[216] =  B00000000;
letters[217] =  B00000000;
letters[218] =  B00000000;
letters[219] =  B00000000;
letters[220] =  B01100110;
letters[221] =  B00000001;
letters[222] =  B00000000;
letters[223] =  B00000000;

// Symbol <
letters[224] =  B00000000;
letters[225] =  B00000000;
letters[226] =  B00000000;
letters[227] =  B10000010;
letters[228] =  B01000100;
letters[229] =  B00101000;
letters[230] =  B00010000;
letters[231] =  B00000000;

// Symbol =
letters[232] =  B00000000;
letters[233] =  B00000000;
letters[234] =  B00000000;
letters[235] =  B00100100;
letters[236] =  B00100100;
letters[237] =  B00100100;
letters[238] =  B00100100;
letters[239] =  B00000000;

// Symbol >
letters[240] =  B00000000;
letters[241] =  B00000000;
letters[242] =  B00010000;
letters[243] =  B00101000;
letters[244] =  B01000100;
letters[245] =  B10000010;
letters[246] =  B00000000;
letters[247] =  B00000000;

// Symbol ?
letters[248] =  B00000000;
letters[249] =  B00000000;
letters[250] =  B01100000;
letters[251] =  B10010000;
letters[252] =  B10001101;
letters[253] =  B10000000;
letters[254] =  B01100000;
letters[255] =  B00000000;

// Symbol @
letters[256] =  B00000000;
letters[257] =  B01111000;
letters[258] =  B10100101;
letters[259] =  B10100101;
letters[260] =  B10100101;
letters[261] =  B10011001;
letters[262] =  B10000001;
letters[263] =  B01011110;

// Letter A
letters[264] = B00000000;
letters[265] = B00111111;
letters[266] = B01001000;
letters[267] = B10001000;
letters[268] = B10001000;
letters[269] = B10001000;
letters[270] = B01001000;
letters[271] = B00111111;

// Letter B
letters[272]  = B00000000;
letters[273]  = B01110110;
letters[274] = B10001001;
letters[275] = B10001001;
letters[276] = B10001001;
letters[277] = B10001001;
letters[278] = B10001001;
letters[279] = B11111111;

// Letter C
letters[280] = B00000000;
letters[281] = B00100100;
letters[282] = B01000010;
letters[283] = B10000001;
letters[284] = B10000001;
letters[285] = B10000001;
letters[286] = B01000010;
letters[287] = B00111100;

// Letter D
letters[288] = B00000000;
letters[289] = B00111100;
letters[290] = B01000010;
letters[291] = B10000001;
letters[292] = B10000001;
letters[293] = B10000001;
letters[294] = B10000001;
letters[295] = B11111111;

// Letter E
letters[296] = B00000000;
letters[297] = B10000001;
letters[298] = B10000001;
letters[299] = B10010001;
letters[300] = B10010001;
letters[301] = B10010001;
letters[302] = B10010001;
letters[303] = B11111111;


// Letter F
letters[304] = B00000000;
letters[305] = B10000000;
letters[306] = B10000000;
letters[307] = B10010000;
letters[308] = B10010000;
letters[309] = B10010000;
letters[310] = B10010000;
letters[311] = B11111111;

// Letter G
letters[312] = B00000000;
letters[313] = B00101100;
letters[314] = B01001010;
letters[315] = B10001001;
letters[316] = B10000001;
letters[317] = B10000001;
letters[318] = B01000010;
letters[319] = B00111100;

// Letter H
letters[320] = B00000000;
letters[321] = B11111111;
letters[322] = B00001000;
letters[323] = B00001000;
letters[324] = B00001000;
letters[325] = B00001000;
letters[326] = B00001000;
letters[327] = B11111111;

// Letter I
letters[328] = B00000000;
letters[329] = B00000000;
letters[330] = B10000001;
letters[331] = B10000001;
letters[332] = B11111111;
letters[333] = B10000001;
letters[334] = B10000001;
letters[335] = B00000000;

// Letter J
letters[336] = B00000000;
letters[337] = B10000000;
letters[338] = B10000000;
letters[339] = B11111100;
letters[340] = B10000010;
letters[341] = B10000001;
letters[342] = B10000001;
letters[343] = B10000010;

// Letter K
letters[344] = B00000000;
letters[345] = B10000001;
letters[346] = B01000010;
letters[347] = B00100100;
letters[348] = B00011000;
letters[349] = B00001000;
letters[350] = B00000100;
letters[351] = B11111111;

// Letter L
letters[352] = B00000000;
letters[353] = B00000001;
letters[354] = B00000001;
letters[355] = B00000001;
letters[356] = B00000001;
letters[357] = B00000001;
letters[358] = B00000001;
letters[359] = B11111111;

// Letter M
letters[360] =  B00000000;
letters[361] =  B01111111;
letters[362] =  B10000000;
letters[363] =  B10000000;
letters[364] = B01110000;
letters[365] = B10000000;
letters[366] = B10000000;
letters[367] = B01111111;

// Letter N
letters[368] =  B00000000;
letters[369] =  B11111111;
letters[370] =  B00000010;
letters[371] =  B00000100;
letters[372] =  B00011000;
letters[373] =  B00100000;
letters[374] =  B01000000;
letters[375] =  B11111111;

// Letter 0
letters[376] =  B00000000;
letters[377] =  B00111100;
letters[378] =  B01000010;
letters[379] =  B10000001;
letters[380] =  B10000001;
letters[381] =  B10000001;
letters[382] =  B01000010;
letters[383] =  B00111100;

// Letter P
letters[384] =  B00000000;
letters[385] =  B00110000;
letters[386] =  B01001000;
letters[387] =  B10000100;
letters[388] =  B10000100;
letters[389] =  B10000100;
letters[390] =  B10000100;
letters[391] =  B11111111;

// Letter Q
letters[392] =  B00000000;
letters[393] =  B00111101;
letters[394] =  B01000010;
letters[395] =  B10000101;
letters[396] =  B10001001;
letters[397] =  B10000001;
letters[398] =  B01000010;
letters[399] =  B00111100;

// Letter R
letters[400] =  B00000000;
letters[401] =  B00110001;
letters[402] =  B01001010;
letters[403] =  B10000100;
letters[404] =  B10000100;
letters[405] =  B10000100;
letters[406] =  B10000100;
letters[407] =  B11111111;

// Letter S
letters[408] =  B00000000;
letters[409] =  B01001110;
letters[410] =  B10010001;
letters[411] =  B10010001;
letters[412] =  B10010001;
letters[413] =  B10010001;
letters[414] =  B10010001;
letters[415] =  B01100110;

// Letter T
letters[416] =  B00000000;
letters[417] =  B10000000;
letters[418] =  B10000000;
letters[419] =  B10000000;
letters[420] =  B11111111;
letters[421] =  B10000000;
letters[422] =  B10000000;
letters[423] =  B10000000;

// Letter U
letters[424] =  B00000000;
letters[425] =  B11111100;
letters[426] =  B00000010;
letters[427] =  B00000001;
letters[428] =  B00000001;
letters[429] =  B00000001;
letters[430] =  B00000010;
letters[431] =  B11111100;

// Letter V
letters[432] =  B00000000;
letters[433] =  B11111000;
letters[434] =  B00000100;
letters[435] =  B00000010;
letters[436] =  B00000001;
letters[437] =  B00000010;
letters[438] =  B00000100;
letters[439] =  B11111000;

// Letter W
letters[440] =  B00000000;
letters[441] =  B11111110;
letters[442] =  B00000001;
letters[443] =  B00000001;
letters[444] =  B00001110;
letters[445] =  B00000001;
letters[446] =  B00000001;
letters[447] =  B11111110;

// Letter X
letters[448] =  B00000000;
letters[449] =  B10000001;
letters[450] =  B01000010;
letters[451] =  B00100100;
letters[452] =  B00011000;
letters[453] =  B00100100;
letters[454] =  B01000010;
letters[455] =  B10000001;

// Letter Y
letters[456] =  B00000000;
letters[457] =  B10000000;
letters[458] =  B01000000;
letters[459] =  B00100000;
letters[460] =  B00011111;
letters[461] =  B00100000;
letters[462] =  B01000000;
letters[463] =  B10000000;

// Letter Z
letters[464] =  B00000000;
letters[465] =  B10000001;
letters[466] =  B11000001;
letters[467] =  B10100001;
letters[468] =  B10010001;
letters[469] =  B10001001;
letters[470] =  B10000101;
letters[471] =  B10000011;

// Symbol !
letters[472] =  B00000000;
letters[473] =  B00000000;
letters[474] =  B00000000;
letters[475] =  B00000000;
letters[476] =  B11110011;
letters[477] =  B00000000;
letters[478] =  B00000000;
letters[479] =  B00000000;

// Symbol "
letters[480] =  B00000000;
letters[481] =  B00000000;
letters[482] =  B00000000;
letters[483] =  B11100000;
letters[484] =  B00000000;
letters[485] =  B11100000;
letters[486] =  B00000000;
letters[487] =  B00000000;

// Symbol #
letters[488] =  B00000000;
letters[489] =  B00100100;
letters[490] =  B11111111;
letters[491] =  B00100100;
letters[492] =  B00100100;
letters[493] =  B00100100;
letters[494] =  B11111111;
letters[495] =  B00100100;

// Symbol $
letters[496] =  B00000000;
letters[497] =  B01001110;
letters[498] =  B10010001;
letters[499] =  B10010001;
letters[500] =  B11111111;
letters[501] =  B10010001;
letters[502] =  B10010001;
letters[503] =  B01100110;

// Symbol %
letters[504] =  B00000000;
letters[505] =  B00000000;
letters[506] =  B01000110;
letters[507] =  B00110000;
letters[508] =  B00011000;
letters[509] =  B00000100;
letters[510] =  B01100010;
letters[511] =  B00000001;

// Symbol &
letters[512] =  B00000000;
letters[513] =  B00000000;
letters[514] =  B00000101;
letters[515] =  B01000010;
letters[516] =  B10100101;
letters[517] =  B10101001;
letters[518] =  B01010001;
letters[519] =  B00101110;

// Symbol '
letters[520] =  B00000000;
letters[521] =  B00000000;
letters[522] =  B00000000;
letters[523] =  B11100000;
letters[524] =  B00000000;
letters[525] =  B00000000;
letters[526] =  B00000000;
letters[527] =  B00000000;

// Symbol (
letters[528] =  B00000000;
letters[529] =  B00000000;
letters[530] =  B10000001;
letters[531] =  B01000010;
letters[532] =  B00100100;
letters[533] =  B00011000;
letters[534] =  B00000000;
letters[535] =  B00000000;

// Symbol )
letters[536] =  B00000000;
letters[537] =  B00000000;
letters[538] =  B00011000;
letters[539] =  B00100100;
letters[540] =  B01000010;
letters[541] =  B10000001;
letters[542] =  B00000000;
letters[543] =  B00000000;

// Symbol *
letters[544] =  B00000000;
letters[545] =  B10010010;
letters[546] =  B01010100;
letters[547] =  B00111000;
letters[548] =  B11111111;
letters[549] =  B00111000;
letters[550] =  B01010100;
letters[551] =  B10010010;

// Symbol +
letters[552] =  B00000000;
letters[553] =  B00010000;
letters[554] =  B00010000;
letters[555] =  B00010000;
letters[556] =  B11111111;
letters[557] =  B00010000;
letters[558] =  B00010000;
letters[559] =  B00010000;

// Symbol '
letters[560] =  B00000000;
letters[561] =  B00000000;
letters[562] =  B00000000;
letters[563] =  B11000000;
letters[564] =  B00100000;
letters[565] =  B00000000;
letters[566] =  B00000000;
letters[567] =  B00000000;

// Symbol -
letters[568] =  B00000000;
letters[569] =  B00000000;
letters[570] =  B00000000;
letters[571] =  B00010000;
letters[572] =  B00010000;
letters[573] =  B00010000;
letters[574] =  B00000000;
letters[575] =  B00000000;

// Symbol .
letters[576] =  B00000000;
letters[577] =  B00000000;
letters[578] =  B00000000;
letters[579] =  B00000011;
letters[580] =  B00000011;
letters[581] =  B00000000;
letters[582] =  B00000000;
letters[583] =  B00000000;

// Symbol /
letters[584] =  B00000000;
letters[585] =  B10000000;
letters[586] =  B01000000;
letters[587] =  B00100000;
letters[588] =  B00011000;
letters[589] =  B00000100;
letters[590] =  B00000010;
letters[591] =  B00000001;

// Number 0 - zero
letters[592] =  B00000000;
letters[593] =  B00111100;
letters[594] =  B01000010;
letters[595] =  B10100001;
letters[596] =  B10010001;
letters[597] =  B10001001;
letters[598] =  B01000010;
letters[599] =  B00111100;

// Number 1
letters[600] =  B00000000;
letters[601] =  B00000000;
letters[602] =  B00000001;
letters[603] =  B11111111;
letters[604] =  B01000001;
letters[605] =  B00100001;
letters[606] =  B00000000;
letters[607] =  B00000000;

// Number 2
letters[608] =  B00000000;
letters[609] =  B01100001;
letters[610] =  B10010001;
letters[611] =  B10001001;
letters[612] =  B10001001;
letters[613] =  B10000101;
letters[614] =  B10000011;
letters[615] =  B01100001;

// Number 3
letters[616] =  B00000000;
letters[617] =  B01111110;
letters[618] =  B10011001;
letters[619] =  B10011001;
letters[620] =  B10011001;
letters[621] =  B10000001;
letters[622] =  B10000001;
letters[623] =  B01000110;

// Number 4
letters[624] =  B00000000;
letters[625] =  B00000100;
letters[626] =  B11111111;
letters[627] =  B01000100;
letters[628] =  B00100100;
letters[629] =  B00010100;
letters[630] =  B00001100;
letters[631] =  B00000100;

// Number 5
letters[632] =  B00000000;
letters[633] =  B10001110;
letters[634] =  B10010001;
letters[635] =  B10010001;
letters[636] =  B10010001;
letters[637] =  B10010001;
letters[638] =  B10010001;
letters[639] =  B11100010;

// Number 6
letters[640] =  B00000000;
letters[641] =  B01001110;
letters[642] =  B10010001;
letters[643] =  B10010001;
letters[644] =  B10010001;
letters[645] =  B10010001;
letters[646] =  B10010001;
letters[647] =  B01111110;

// Number 7
letters[648] =  B00000000;
letters[649] =  B11100000;
letters[650] =  B10010000;
letters[651] =  B10001000;
letters[652] =  B10000111;
letters[653] =  B00000000;
letters[654] =  B00000000;
letters[655] =  B00000000;

// Number 8
letters[656] =  B00000000;
letters[657] =  B01100110;
letters[658] =  B10011001;
letters[659] =  B10011001;
letters[660] =  B10011001;
letters[661] =  B10011001;
letters[662] =  B10011001;
letters[663] =  B01100110;

// Number 9
letters[664] =  B00000000;
letters[665] =  B01111110;
letters[666] =  B10001001;
letters[667] =  B10001001;
letters[668] =  B10001001;
letters[669] =  B10001001;
letters[670] =  B10001001;
letters[671] =  B01110010;


for (int i = 0; i < 8; i++){
  led[i] = letters[i];
  currentdisplay[i] = letters[i];
  }
}

void loop() {
currentCode = "1";
count = 0;
out = 0;
while(out == 0){
char customKey = customKeypad.getKey();
char newcustomKey = customKey;
if(customKey){

  if(customKey == '1'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=136; i <144; i++){
  led[i-136] = letters[i];
  currentdisplay[i-136] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == '2'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=144; i <152; i++){
  led[i-144] = letters[i];
  currentdisplay[i-144] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == '3'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=152; i <160; i++){
  led[i-152] = letters[i];
  currentdisplay[i-152] = letters[i];;
  }
  screenUpdate();
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == '4'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=160; i <168; i++){
  led[i-160] = letters[i];
  currentdisplay[i-160] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == '5'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=168; i <176; i++){
  led[i-168] = letters[i];
  currentdisplay[i-168] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == '6'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=176; i <184; i++){
  led[i-176] = letters[i];
  currentdisplay[i-176] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == '7'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=184; i <192; i++){
  led[i-184] = letters[i];
  currentdisplay[i-184] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == '8'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=192; i <200; i++){
  led[i-192] = letters[i];
  currentdisplay[i-192] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == '9'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=200; i <208; i++){
  led[i-200] = letters[i];
  currentdisplay[i-200] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == 'A'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=264; i <272; i++){
  led[i-264] = letters[i];
  currentdisplay[i-264] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == 'B'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=272; i <280; i++){
  led[i-272] = letters[i];
  currentdisplay[i-272] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == 'C'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=280; i <288; i++){
  led[i-280] = letters[i];
  currentdisplay[i-280] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
 if(customKey == 'D'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=288; i <296; i++){
  led[i-288] = letters[i];
  currentdisplay[i-288] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == '0'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=128; i <136; i++){
  led[i-128] = letters[i];
  currentdisplay[i-128] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == '#'){
  // Clear the screen before starting
  count++;
  tempString = String(customKey);
  currentCode += tempString;
  Serial.println(count);
  while(customKey){
  newcustomKey = customKeypad.getKey();
  for (int i=24; i <32; i++){
  led[i-24] = letters[i];
  currentdisplay[i-24] = letters[i];;
  }
  screenUpdate();
  
  
  if(newcustomKey){
    customKey = newcustomKey;
    break;
  }
  }
  }
  if(customKey == '*'){
  out = 1;
  }


}
}
if(currentCode == passCode){

digitalWrite(greenPin,HIGH);
digitalWrite(Relay,HIGH);
for(int x = 0; x<2500; x++){
  for (int i=80; i <88; i++){
  led[i-80] = letters[i];
  currentdisplay[i-80] = letters[i];;
  }
  screenUpdate();
  }
digitalWrite(greenPin,LOW);
digitalWrite(Relay,LOW);
Serial.println(currentCode);
}
else{
digitalWrite(redPin,HIGH);
for(int x = 0; x<1500; x++){
  for (int i=24; i <32; i++){
  led[i-24] = letters[i];
  currentdisplay[i-24] = letters[i];;
  }
  screenUpdate();
  }
digitalWrite(redPin,LOW);
Serial.println(currentCode);
}
}
// End Reading Serial Data 


void screenUpdate() {
uint8_t row = B00000001;

for (byte k = 0; k < 9; k++) {
// Open up the latch ready to receive data
digitalWrite(latchPin, LOW);
shiftIt(~row );
shiftIt(led[k] ); // LED array

// Close the latch, sending the data in the registers out to the matrix
digitalWrite(latchPin, HIGH);
row = row << 1;
  }
}

void shiftIt(byte dataOut) {
// Shift out 8 bits LSB first,
// on rising edge of clock

boolean pinState;

//clear shift register read for sending data
digitalWrite(dataPin, LOW);

// for each bit in dataOut send out a bit
for (int i=0; i<8; i++) {

  //set clockPin to LOW prior to sending bit
digitalWrite(clockPin, LOW);

// if the value of DataOut and (logical AND) a bitmask
// are true, set pinState to 1 (HIGH)
if ( dataOut & (1<<i) ) {
pinState = HIGH;
}
else {
  pinState = LOW;
}

//sets dataPin to HIGH or LOW depending on pinState
digitalWrite(dataPin, pinState);

//send bit out on rising edge of clock
digitalWrite(clockPin, HIGH);
digitalWrite(dataPin, LOW);
}

//stop shifting
digitalWrite(clockPin, LOW);
}