#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <logging.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <wiringPi.h>
#include <PCA9685.h>
#include <car.h>

CarInfoStruct  CarInfo;   // settings for car
unsigned       hPCA9685;  // handle to PCA9685

void CarDefaultSettings()
{
  log_function("CarDefaultSettings()...\n");

  // Initialize CarInfo with defaults
  CarInfo.address      = 0x40;
  CarInfo.frequency    = 0xFF;

  CarInfo.piLeftA      = 0x00;
  CarInfo.piLeftB      = 0x01;
  CarInfo.piRightA     = 0x02;
  CarInfo.piRightB     = 0x03;
  CarInfo.pwmLeft      = 0x04;
  CarInfo.pwmRight     = 0x05;
  CarInfo.minDrive     = 0x0;
  CarInfo.maxDrive     = 0x0FFF;
  CarInfo.reverseLeft  = 0;
  CarInfo.reverseRight = 0;

  CarInfo.pwmSteering  = 0x00;
  CarInfo.minSteering  = 0;
  CarInfo.maxSteering  = 0x0FFF;
  CarInfo.homeSteering = 0x0800;

  CarInfo.pwmPan       = 0x0E;
  CarInfo.minPan       = 0;
  CarInfo.maxPan       = 0x0FFF;
  CarInfo.homePan      = 0x0800;

  CarInfo.pwmTilt      = 0x0F;
  CarInfo.minTilt      = 0;
  CarInfo.maxTilt      = 0x0FFF;
  CarInfo.homeTilt     = 0x0800;

  log_function("CarDefaultSettings DONE\n");
  return;
}


int CarWriteSettings(char *fileName)
{
  log_function("CarWriteSettings(fileName: %s)...\n", fileName);

  xmlDoc  *doc;
  xmlNode *rootNode;
  xmlNode *node = NULL;
  xmlNode *node1 = NULL;
  xmlNode *node2 = NULL;
  char    buffer[256];

  // Create XML document
  doc = xmlNewDoc(BAD_CAST "1.0");
  rootNode = xmlNewNode(NULL, BAD_CAST "car");
  xmlDocSetRootElement(doc, rootNode);

  sprintf(buffer, "0x%02X", CarInfo.address);
  xmlNewTextChild(rootNode, NULL, BAD_CAST "address", buffer);
  sprintf(buffer, "0x%02X", CarInfo.frequency);
  xmlNewTextChild(rootNode, NULL, BAD_CAST "frequency", buffer);

  node = xmlNewChild(rootNode, NULL, BAD_CAST "drive", NULL);
  sprintf(buffer, "0x%04X", CarInfo.minDrive);
  xmlNewTextChild(node, NULL, BAD_CAST "min", buffer);
  sprintf(buffer, "0x%04X", CarInfo.maxDrive);
  xmlNewTextChild(node, NULL, BAD_CAST "max", buffer);

  node1 = xmlNewChild(node, NULL, BAD_CAST "address", NULL);
  node2 = xmlNewChild(node1, NULL, BAD_CAST "RaspberryPi", NULL);
  sprintf(buffer, "0x%02X", CarInfo.piLeftA);
  xmlNewTextChild(node2, NULL, BAD_CAST "leftA", buffer);
  sprintf(buffer, "0x%02X", CarInfo.piLeftB);
  xmlNewTextChild(node2, NULL, BAD_CAST "leftB", buffer);
  sprintf(buffer, "0x%02X", CarInfo.piRightA);
  xmlNewTextChild(node2, NULL, BAD_CAST "rightA", buffer);
  sprintf(buffer, "0x%02X", CarInfo.piRightB);
  xmlNewTextChild(node2, NULL, BAD_CAST "rightB", buffer);
  node2 = xmlNewChild(node1, NULL, BAD_CAST "PWM", NULL);
  sprintf(buffer, "0x%02X", CarInfo.pwmLeft);
  xmlNewTextChild(node2, NULL, BAD_CAST "left", buffer);
  sprintf(buffer, "0x%02X", CarInfo.pwmRight);
  xmlNewTextChild(node2, NULL, BAD_CAST "right", buffer);

  node1 = xmlNewChild(node, NULL, BAD_CAST "reverse", NULL);
  sprintf(buffer, "%s", CarInfo.reverseLeft ? "True" : "False");
  xmlNewTextChild(node1, NULL, BAD_CAST "left", buffer);
  sprintf(buffer, "%s", CarInfo.reverseRight ? "True" : "False");
  xmlNewTextChild(node1, NULL, BAD_CAST "right", buffer);

  node1 = xmlNewChild(rootNode, NULL, BAD_CAST "steering", NULL);
  sprintf(buffer, "0x%02X", CarInfo.pwmSteering);
  xmlNewTextChild(node1, NULL, BAD_CAST "pwmAddress", buffer);
  sprintf(buffer, "0x%04X", CarInfo.minSteering);
  xmlNewTextChild(node1, NULL, BAD_CAST "min", buffer);
  sprintf(buffer, "0x%04X", CarInfo.maxSteering);
  xmlNewTextChild(node1, NULL, BAD_CAST "max", buffer);
  sprintf(buffer, "0x%04X", CarInfo.homeSteering);
  xmlNewTextChild(node1, NULL, BAD_CAST "home", buffer);

  node1 = xmlNewChild(rootNode, NULL, BAD_CAST "pan", NULL);
  sprintf(buffer, "0x%02X", CarInfo.pwmPan);
  xmlNewTextChild(node1, NULL, BAD_CAST "pwmAddress", buffer);
  sprintf(buffer, "0x%04X", CarInfo.minPan);
  xmlNewTextChild(node1, NULL, BAD_CAST "min", buffer);
  sprintf(buffer, "0x%04X", CarInfo.maxPan);
  xmlNewTextChild(node1, NULL, BAD_CAST "max", buffer);
  sprintf(buffer, "0x%04X", CarInfo.homePan);
  xmlNewTextChild(node1, NULL, BAD_CAST "home", buffer);

  node1 = xmlNewChild(rootNode, NULL, BAD_CAST "tilt", NULL);
  sprintf(buffer, "0x%02X", CarInfo.pwmTilt);
  xmlNewTextChild(node1, NULL, BAD_CAST "pwmAddress", buffer);
  sprintf(buffer, "0x%04X", CarInfo.minTilt);
  xmlNewTextChild(node1, NULL, BAD_CAST "min", buffer);
  sprintf(buffer, "0x%04X", CarInfo.maxTilt);
  xmlNewTextChild(node1, NULL, BAD_CAST "max", buffer);
  sprintf(buffer, "0x%04X", CarInfo.homeTilt);
  xmlNewTextChild(node1, NULL, BAD_CAST "home", buffer);

  // Write it to config file
  xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 1);

  // Clean up
  xmlFreeDoc(doc);
  xmlCleanupParser();
  
  log_function("CarWriteSettings DONE\n");
  return 1;
}

void CarProcessSettingsNode(xmlNode *node, char *section)
{
  log_function("CarProcessSettingsNode(xmlNode: %s, section: %s)...\n", node->name, section);

  xmlNode      *curNode;
  xmlBufferPtr buffer;

  buffer = xmlBufferCreateSize(40);

  for (curNode = node; curNode; curNode = curNode->next)
  {
    if (xmlNodeIsText(curNode) && curNode->children == NULL)
    {
      // Get content
      xmlNodeBufGetContent(buffer, curNode);

      // If content isn't blank, process it
      if (xmlBufferLength(buffer) > 0)
      {
        if(xmlStrcasecmp(section, "car/address") == 0)
          CarInfo.address = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/frequency") == 0)
          CarInfo.frequency = (int)strtol(xmlBufferContent(buffer), NULL, 0);

        else if(xmlStrcasecmp(section, "car/drive/min") == 0)
          CarInfo.minDrive = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/drive/max") == 0)
          CarInfo.maxDrive = (int)strtol(xmlBufferContent(buffer), NULL, 0);

        else if(xmlStrcasecmp(section, "car/drive/address/RaspberryPi/leftA") == 0)
          CarInfo.piLeftA = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/drive/address/RaspberryPi/leftB") == 0)
          CarInfo.piLeftB = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/drive/address/RaspberryPi/rightA") == 0)
          CarInfo.piRightA = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/drive/address/RaspberryPi/rightB") == 0)
          CarInfo.piRightB = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/drive/address/PWM/left") == 0)
          CarInfo.pwmLeft = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/drive/address/PWM/right") == 0)
          CarInfo.pwmRight = (int)strtol(xmlBufferContent(buffer), NULL, 0);

        else if(xmlStrcasecmp(section, "car/drive/reverse/left") == 0)
        {
          if(xmlStrcasecmp(xmlBufferContent(buffer), "0") == 0 ||
             xmlStrcasecmp(xmlBufferContent(buffer), "False") == 0)
            CarInfo.reverseLeft = 0;
          else
            CarInfo.reverseLeft = 1;
        }
        else if(xmlStrcasecmp(section, "car/drive/reverse/right") == 0)
        {
          if(xmlStrcasecmp(xmlBufferContent(buffer), "0") == 0 ||
             xmlStrcasecmp(xmlBufferContent(buffer), "False") == 0)
            CarInfo.reverseRight = 0;
          else
            CarInfo.reverseRight = 1;
        }

        else if(xmlStrcasecmp(section, "car/steering/pwmAddress") == 0)
          CarInfo.pwmSteering = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/steering/min") == 0)
          CarInfo.minSteering = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/steering/max") == 0)
          CarInfo.maxSteering = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/steering/home") == 0)
          CarInfo.homeSteering = (int)strtol(xmlBufferContent(buffer), NULL, 0);

        else if(xmlStrcasecmp(section, "car/tilt/pwmAddress") == 0)
          CarInfo.pwmTilt = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/tilt/min") == 0)
          CarInfo.minTilt = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/tilt/max") == 0)
          CarInfo.maxTilt = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/tilt/home") == 0)
          CarInfo.homeTilt = (int)strtol(xmlBufferContent(buffer), NULL, 0);

        else if(xmlStrcasecmp(section, "car/pan/pwmAddress") == 0)
          CarInfo.pwmPan = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/pan/min") == 0)
          CarInfo.minPan = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/pan/max") == 0)
          CarInfo.maxPan = (int)strtol(xmlBufferContent(buffer), NULL, 0);
        else if(xmlStrcasecmp(section, "car/pan/home") == 0)
          CarInfo.homePan = (int)strtol(xmlBufferContent(buffer), NULL, 0);

      }
    }

    if (curNode->children != NULL)
    {
      // Recurse
      char newSection[256];
      sprintf(newSection, "%s%s%s", section, *section == '\0' ? "" : "/", curNode->name);
      CarProcessSettingsNode(curNode->children, newSection);
    }

  }

  // Cleanup buffer
  xmlBufferFree(buffer);

  log_function("CarProcessSettingsNode DONE\n");
}


int CarReadSettings(char *fileName)
{
  log_function("CarReadSettings(fileName: %s)...\n", fileName);

  xmlDoc  *doc;
  xmlNode *rootElement;
  xmlNode *curNode;

  // Start by setting defaults (in case any settings are missing)
  CarDefaultSettings();

  doc = xmlReadFile(fileName, NULL, 0);
  if (doc == NULL)
  {
    log_message(LOG_NORMAL, "Config: %s does not exist.  Using defaults...\n", fileName);

    log_function("CarReadSettings DONE\n");
    return 0;
  }

  // Process file
  log_message(LOG_NORMAL, "Config: %s found.  Processing...\n", fileName);

  // Process all the elements
  rootElement = xmlDocGetRootElement(doc);
  CarProcessSettingsNode(rootElement, "");

  xmlFreeDoc(doc);
  xmlCleanupParser();

  log_function("CarReadSettings DONE\n");
  return 1;
}


int CarInit()
{
  log_function("CarInit()...\n");

  int status;

  // Read settings into CarInfo
  CarReadSettings(CAR_CONFIGFILE);
  CarWriteSettings(CAR_CONFIGFILE);

  pinMode(CarInfo.piLeftA, OUTPUT);
  pinMode(CarInfo.piLeftB, OUTPUT);
  pinMode(CarInfo.piRightA, OUTPUT);
  pinMode(CarInfo.piRightB, OUTPUT);

  if ((hPCA9685 = PCA9685Init(0x40, 100)) == -1)
  {
    log_message(LOG_ERROR, "Error: %d\n       %s\n", errno, strerror(errno));
    return 1;
  }


  if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmLeft, 0)) != 0)
    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
  if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmRight, 0)) != 0)
    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));

  if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmPan, 200)) != 0)
    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
  delay(1000);

  if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmPan, 750)) != 0)
    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
  delay(1000);
  
  if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmPan, 1200)) != 0)
    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
  delay(1000);

/*
  int q;
  for(q = 4096; q > 0; q -= 5)
  {
    if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmPan, q)) != 0)
      fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
    delay(1);
  }
*/

  if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmSteering, 0)) != 0)
    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
  if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmPan, 0)) != 0)
    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
  if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmTilt, 0)) != 0)
    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));

//  if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmSteering, CarInfo.homeSteering)) != 0)
//    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
//  if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmTilt, CarInfo.homeTilt)) != 0)
//    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
//  if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmPan, CarInfo.homePan)) != 0)
//    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));

/*
  digitalWrite(CarInfo.piLeftA, HIGH);
  digitalWrite(CarInfo.piLeftB, LOW);
  digitalWrite(CarInfo.piRightA, HIGH);
  digitalWrite(CarInfo.piRightB, LOW);

  int q;
  for(q = 4096; q > 0; q -= 5)
  {
    if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmLeft, q)) != 0)
      fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
    if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmRight, q)) != 0)
      fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
    delay(1);
  }

  delay(1000);

  digitalWrite(CarInfo.piLeftA, LOW);
  digitalWrite(CarInfo.piLeftB, HIGH);
  digitalWrite(CarInfo.piRightA, LOW);
  digitalWrite(CarInfo.piRightB, HIGH);

  for(q = 0; q <= 4096; q += 5)
  {
    if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmLeft, q)) != 0)
      fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
    if((status = PCA9685WriteLedDuty(hPCA9685, CarInfo.pwmRight, q)) != 0)
      fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
    delay(1);
  }

  delay(1000);

  digitalWrite(CarInfo.piLeftA, LOW);
  digitalWrite(CarInfo.piLeftB, LOW);
  digitalWrite(CarInfo.piRightA, LOW);
  digitalWrite(CarInfo.piRightB, LOW);
*/
  log_function("CarInit DONE\n");
    
}
