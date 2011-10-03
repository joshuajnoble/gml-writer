/*
 *  gml_writer.h
 *  
 *
 *  Created by base on 30/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "GMLWriter.h"

const char GMLWriter::tagBegin[38] = "<gml spec='1.0'><tag><drawing>";
const char GMLWriter::tagEnd[23] = "</drawing></tag></gml>";

GMLWriter::GMLWriter() {

}

GMLWriter::~GMLWriter() {
  file.close();
}

void GMLWriter::init()
{
  if (!sd.init(SPI_FULL_SPEED, 10)) sd.initErrorHalt();

  // first check that we don't already have stuff on the card - critical
  int n = 1;
  char name[8];

  String s = n;
  s += ".GML";

  Serial.print(" doesnt ");
  Serial.print( s.c_str() );
  
  while( sd.exists( s.c_str() )) 
  {
    Serial.print(" exists ");
    n++;
    s = n;
    s += ".GML";
  }

  if(!file.open(s.c_str(), O_WRITE | O_CREAT )) {
    sd.errorHalt("can't create new file");
  }
}

void GMLWriter::beginStroke() {

  file.write("<stroke>");
}

void GMLWriter::endStroke() {

  file.write("</stroke>");
}

void GMLWriter::beginDrawing() {

  file.write(tagBegin);
}

void GMLWriter::endDrawing() {

  file.write(tagEnd);

  file.close();
}

void GMLWriter::addPoint( float x, float y, float z ) {	
  String s;
  char fp[10];
  s += "<pt><x>";
  //sprintf(&fp[0],  "%f", x);
  floatToString(&fp[0], x, 5);
  Serial.print(fp);
  //itoa(x, fp, 10);
  s += fp;
  s +=  "</x><y>";
  //sprintf(&fp[0], 5, "%f", y);
  floatToString(&fp[0], y, 5);
  Serial.print(fp);
  s += fp;
  s +=  "</y><z>";
  //snprintf(&fp[0], 5, "%f", z);
  floatToString(&fp[0], z, 5);
  Serial.print(fp);
  s += fp;
  s +=  "</z></pt>";

  file.write(s.c_str());
}

void GMLWriter::addPoint( float x, float y, float z, float time ) {
  char fp[5];
  String s;
  s += "<pt><x>";
  snprintf(&fp[0], 5, "%f", x);
  s += fp;
  s +=  "</x><y>";
  snprintf(&fp[0], 5, "%f", y);
  s += fp;
  s +=  "</y><z>";
  snprintf(&fp[0], 5, "%f", z);
  s += fp;
  s +=  "</z>";
  s += "<t>";
  snprintf(&fp[0], 5, "%f", time);
  s += fp;
  s += "</t></pt>";
  file.write(s.c_str());
}

void GMLWriter::addPoint( float x, float y, float z, float time, float rotation ) {
  String s;
  char fp[5];
  s += "<pt><rot>";
  snprintf(&fp[0], 5, "%f", rotation);
  s += fp;
  s+= fp;
  s+="</rot>";
  s += "<x>";
  snprintf(&fp[0], 5, "%f", x);
  s += fp;
  s +=  "</x><y>";
  snprintf(&fp[0], 5, "%f", y);
  s += fp;
  s +=  "</y><z>";
  snprintf(&fp[0], 5, "%f", z);
  s += fp;
  s +=  "</z>";
  s += "<t>";
  snprintf(&fp[0], 5, "%f", time);
  s += fp;
  s += "</t></pt>";
  file.write(s.c_str());
}

int GMLWriter::getAsString(char *pt, int len) {

  int written = 0;
  char data;
  while ( (data = file.read()) > 0 && written < len ) {
    pt[len] = data;
    readCount++;
    written++;
  }

  if(written < len) {
    return 1; // read complete
  } 
  else {
    return 0; // read incomplete
  }

}

char * GMLWriter::floatToString(char * outstr, float value, int places, int minwidth, bool rightjustify) {
    // this is used to write a float value to string, outstr.  oustr is also the return value.
    int digit;
    float tens = 0.1;
    int tenscount = 0;
    int i;
    float tempfloat = value;
    int c = 0;
    int charcount = 1;
    int extra = 0;
    // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
    // if this rounding step isn't here, the value  54.321 prints as 54.3209

    // calculate rounding term d:   0.5/pow(10,places)  
    float d = 0.5;
    if (value < 0)
        d *= -1.0;
    // divide by ten for each decimal place
    for (i = 0; i < places; i++)
        d/= 10.0;    
    // this small addition, combined with truncation will round our values properly 
    tempfloat +=  d;

    // first get value tens to be the large power of ten less than value    
    if (value < 0)
        tempfloat *= -1.0;
    while ((tens * 10.0) <= tempfloat) {
        tens *= 10.0;
        tenscount += 1;
    }

    if (tenscount > 0)
        charcount += tenscount;
    else
        charcount += 1;

    if (value < 0)
        charcount += 1;
    charcount += 1 + places;

    minwidth += 1; // both count the null final character
    if (minwidth > charcount){        
        extra = minwidth - charcount;
        charcount = minwidth;
    }

    if (extra > 0 and rightjustify) {
        for (int i = 0; i< extra; i++) {
            outstr[c++] = ' ';
        }
    }

    // write out the negative if needed
    if (value < 0)
        outstr[c++] = '-';

    if (tenscount == 0) 
        outstr[c++] = '0';

    for (i=0; i< tenscount; i++) {
        digit = (int) (tempfloat/tens);
        itoa(digit, &outstr[c++], 10);
        tempfloat = tempfloat - ((float)digit * tens);
        tens /= 10.0;
    }

    // if no places after decimal, stop now and return

    // otherwise, write the point and continue on
    if (places > 0)
    outstr[c++] = '.';


    // now write out each decimal place by shifting digits one by one into the ones place and writing the truncated value
    for (i = 0; i < places; i++) {
        tempfloat *= 10.0; 
        digit = (int) tempfloat;
        itoa(digit, &outstr[c++], 10);
        // once written, subtract off that digit
        tempfloat = tempfloat - (float) digit; 
    }
    if (extra > 0 and not rightjustify) {
        for (int i = 0; i< extra; i++) {
            outstr[c++] = ' ';
        }
    }


    outstr[c++] = '\0';
    return outstr;
}


