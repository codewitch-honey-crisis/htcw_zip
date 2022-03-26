// Demo requires an ESP32 (tested) or perhaps an ESP8266 (untested)
#include <Arduino.h>
#include <SPIFFS.h>
#include <htcw_zip.hpp>
using namespace io;
using namespace zip;
void setup() {
    char path[1024];
    Serial.begin(115200);
    SPIFFS.begin(false);
    File f_in = SPIFFS.open("/test.zip","rb");
    file_stream fs(f_in);
    archive arch;
    archive::open(&fs,&arch);
    for(int i = 0;i<arch.entries_size();++i) {
        archive_entry entry;
        arch.entry(i,&entry);
        entry.copy_path(path+1,1024);
        Serial.print("File: ");
        Serial.println(path+1);
        path[0]='/';
        File f_out = SPIFFS.open(path,"wb");
        file_stream fs_out(f_out);
        entry.extract(&fs_out);
        fs_out.close();
        f_out = SPIFFS.open(path,"r");
        while(f_out.available()) {
            int i = f_out.read();
            char sz[2];
            sz[1]=0;
            if(-1!=i) {
                sz[0]=(char)i;
                Serial.print(sz);
            } else {
                break;
            }    
        }
        Serial.println();
    }
    fs.close();
}
void loop() {
    
}