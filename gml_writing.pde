#include <SdFat.h>
#include "GMLWriter.h"

GMLWriter writer;

void setup()
{
  
  Serial.begin(14400);
  Serial.println(" preparing ");
  writer.init();
  Serial.println(" begin ");
  writer.beginDrawing();
  
  writer.addPoint(0.01, 0.02, 0.04);
  delay(100);
  writer.addPoint(0.01, 0.02, 0.04);
  delay(100);
  writer.addPoint(0.01, 0.02, 0.04);
  delay(100);
  writer.addPoint(0.01, 0.02, 0.04);  
  delay(100);
  writer.addPoint(0.01, 0.02, 0.04);
  delay(100);
  writer.addPoint(0.01, 0.02, 0.04);
  delay(100);
  writer.addPoint(0.01, 0.02, 0.04);
  delay(100);
  writer.addPoint(0.01, 0.02, 0.04);
  delay(100);
  writer.addPoint(0.01, 0.02, 0.04);
  delay(100);
  writer.addPoint(0.01, 0.02, 0.04);
  delay(100);
  writer.addPoint(0.01, 0.02, 0.04);
  delay(100);
  writer.addPoint(0.01, 0.02, 0.04);
  delay(100);
  
  Serial.println(" end ");
  writer.endDrawing();
  Serial.println(" done ");
  
}

void loop()
{
}
