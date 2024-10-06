// Master SPI Configuration for Arduino UNO
const int SS1 = 10;  // SS for Slave 1
const int SS2 = 9;   // SS for Slave 2

void SPI_MasterInit() {
  // Set MOSI, SCK as Output, MISO as Input
  DDRB |= (1 << PB3) | (1 << PB5);  // MOSI, SCK output
  DDRB &= ~(1 << PB4);              // MISO input

  // Set SS1 and SS2 as Output
  pinMode(SS1, OUTPUT);
  pinMode(SS2, OUTPUT);
  
  // Disable both slaves initially (pull SS high)
  digitalWrite(SS1, HIGH);
  digitalWrite(SS2, HIGH);
  
  // Enable SPI, Set as Master, Set clock rate fosc/16
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

char SPI_MasterTransmit(char data) {
  //digitalWrite(slaveSSPin, LOW);
  // Load data into the SPI Data Register
  SPDR = data;
  
  // Wait for transmission to complete (SPIF set in SPSR)
  while (!(SPSR & (1 << SPIF)));
   char data_value= SPDR;

  //digitalWrite(slaveSSPin, HIGH);
 
  // Return the data received during the transmission
  return data_value;
}

void communicateWithSlave(int slaveSSPin, const char* message) {
  char receivedMessage[6];
  int i = 0;

  // Select the slave by pulling the SS line low
  digitalWrite(slaveSSPin, LOW);
  
  // Transmit and receive data
  while (message[i] != '\0') {
    char receivedChar = SPI_MasterTransmit(message[i]);
    receivedMessage[i] = receivedChar;
    i++;
    //delay(10);

  }
  
  // Deselect the slave by pulling the SS line high
  digitalWrite(slaveSSPin, HIGH);
  // Null terminate the received message
  receivedMessage[i] = '\0';

  // Print the received message from the slave
  Serial.print("Received from slave: ");
  Serial.println(receivedMessage);
}

void setup() {
  // Initialize the SPI communication as Master
  SPI_MasterInit();
  
  // Begin Serial communication
  Serial.begin(9600);

}

void loop() {
  // Communicate with Slave 1 (send "HELLO" and receive response)
  
 communicateWithSlave(SS1,"HELLO");
  delay(1000);
  
  // Communicate with Slave 2 (send "WORLD" and receive response)
  
  communicateWithSlave(SS2, "WORLD");
  delay(1000);
}
