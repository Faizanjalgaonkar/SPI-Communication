// Slave SPI Configuration for Arduino UNO
#define MESSAGE_LENGTH 6 // Length of the message + null terminator

void SPI_SlaveInit() {
  // Set MISO as Output
  DDRB |= (1 << PB4);  // MISO output
  // Set MOSI, SCK as Input
  DDRB &= ~(1 << PB3) & ~(1 << PB5);
  // Set SS as Input
  DDRB &= ~(1 << PB2);

  // Enable SPI in Slave mode
  SPCR = (1 << SPE);
}

char SPI_SlaveTransmit(char data) {
  // Load the data into the SPI Data Register
  SPDR = data;
  
  // Wait for reception to complete (SPIF set in SPSR)
  while (!(SPSR & (1 << SPIF)));
  char data_value= SPDR;
  
  // Return the data received during the transmission
  return data_value;
}

void setup() {
  // Initialize the SPI communication as Slave
  SPI_SlaveInit();
  
  // Begin Serial communication to display received messages
  Serial.begin(9600);
}

void loop() {
  // Transmit and receive data from the master
  const char* slaveMessage = "WORLD";
  char masterMessage[MESSAGE_LENGTH];
  int i = 0;
  
  // Send and receive data
  while (slaveMessage[i] != '\0') {
    masterMessage[i] = SPI_SlaveTransmit(slaveMessage[i]);
    i++;
  }
  
  // Null terminate the received message
  masterMessage[i] = '\0';

  // Print the message received from the master
  Serial.print("Master sent: ");
  Serial.println(masterMessage);
}
