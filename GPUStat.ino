/*

  __________________ ____ ___  _________ __          __
  /  _____/\______   \    |   \/   _____//  |______ _/  |_
  /   \  ___ |     ___/    |   /\_____  \\   __\__  \\   __\
  \    \_\  \|    |   |    |  / /        \|  |  / __ \|  |
  \______  /|____|   |______/ /_______  /|__| (____  /__|
        \/                           \/           \/

*/
void DisplayGPUStat () {

  //--------------------------------------- Display Background ----------------------------------------------------

  display.fillRect(0, 0, 128 , 64, BLACK);
  /* OLED Background */
  display.setTextSize(2); //set background txt font size
  display.setCursor(1, 12);
  display.println("GPU");

  /*GPU  Clearing Boxes, eg: display.fillRect(<X>, ^Y^, W, H, Color);*/
  //display.fillRect(38, 11, 92, 25, BLACK); // fill CPU live stats section only
  //display.fillRect(0, 0, 128 , 64, BLACK);
  /* Position Test*/
  //display.drawRect(38, 11, 90, 16, WHITE); // fill CPU live stats section only
  //display.drawRect(38, 37, 90, 16, WHITE); // fill GPU live stats section only
  //display.drawRect(38, 54, 36, 10, WHITE); // fill RAM live stats section only
  //display.drawRect(68, 54, 58, 10, WHITE); // Flash bottom right corner when updating

  //---------------------------------------GPU Hardware ID----------------------------------------------------

  /*GPU Hardware ID*/

  /* GPU Manual Position Test*/
  //display.setTextSize(0);
  //display.setCursor(-42, 28); // Negative spacing so, GPU ID doesn't cause a rollover, on the next line
  //String gpuName = "Nvidia GeForce GTX 1080"; // Name spacing test

  //display.setTextSize(0); // string font size
  //String gpuName = "";
  //display.println(gpuName);

  if (inputString.indexOf("GPU") > -1)
  {
    display.setTextSize(1);

    display.setCursor(0, 1);
    //display.setCursor(-41, 28); // Negative spacing so, Nvidia doesn't cause a rollover, on the next line
    int gpuNameStart = inputString.indexOf("GPU:");
    if (inputString.indexOf("NVIDIA", gpuNameStart) > -1) {
      gpuNameStart = gpuNameStart + 11;
    }
    else {
      gpuNameStart = gpuNameStart + 8;
    }


    int gpuNameEnd = inputString.indexOf("CT:", gpuNameStart);
    String gpuName = inputString.substring(gpuNameStart, gpuNameEnd);
    display.println(gpuName);
  }


  //------------------------------------------ GPU Load/Temp -------------------------------------------------

/*GPU Display String*/
  int gpuStringStart = inputString.indexOf("GT:");
  int gpuLoad = inputString.indexOf("GL:");
  int gpuStringLimit = inputString.indexOf("RAM:");
  String gpuString1 = inputString.substring(gpuStringStart + 3, gpuLoad);
  String gpuString2 = inputString.substring(gpuLoad + 3, gpuStringLimit);

  /*GPU TEMPERATURE*/
  display.setTextSize(3);
  display.setCursor(42, 12);
  display.print(gpuString1);
  display.setTextSize(1);

#ifdef noDegree
  display.print("C");       // Centigrade Symbol
#else
  display.print((char)247); //Degrees Symbol
  display.print("C");       // Centigrade Symbol
#endif

  /*GPU LOAD*/
  display.setCursor(0, 35);
  display.setTextSize(2);
  display.print(gpuString2);
  display.setTextSize(1);
  display.println("%");      // Small Percent Symbol

  //--------------------------Trigger an event when CPU or GPU threshold is met ---------------------------------


#ifdef uVol_enableThesholdtriggers

  //uVol_TriggerCPU_temp( cpuString1.toInt() ); // Neopixel CPU  Temperature
  uVol_TriggerCPU_load( cpuString2.toInt() ); // Neopixel CPU  Load

  //uVol_TriggerGPU_temp( gpuString1.toInt() ); // Neopixel GPU  Temperature
  uVol_TriggerGPU_load( gpuString2.toInt() ); // Neopixel GPU  Load

#endif

#ifdef enableCustomThesholdtriggers

  CustomTriggerCPU_temp( cpuString1.toInt() ); // Neopixel CPU  Temperature
  CustomTriggerCPU_load( cpuString2.toInt() ); // Neopixel CPU  Load

  CustomTriggerGPU_temp( gpuString1.toInt() ); // Neopixel GPU  Temperature
  CustomTriggerGPU_load( gpuString2.toInt() ); // Neopixel GPU  Load

#endif

#ifdef enableNeopixelGauges

  CPU_loadGauge( cpuString2.toInt() ); // Neopixel Ring Gauge  CPU  Load
  //CPU_tempGauge( cpuString1.toInt() ); // Neopixel Ring Gauge  CPU  Temperature

  GPU_loadGauge( gpuString2.toInt() ); // Neopixel Ring Gauge  GPU  Load
  //GPU_tempGauge( gpuString1.toInt() ); // Neopixel Ring Gauge  GPU  Temperature

#endif

  display.setTextSize(1);
  display.setCursor(115, 1);
  display.println((char)253);
  display.display();
}
