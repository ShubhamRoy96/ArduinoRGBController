/*
 
___________________ ____ ___  _________ __          __   
\_   ___ \______   \    |   \/   _____//  |______ _/  |_ 
/    \  \/|     ___/    |   /\_____  \\   __\__  \\   __\
\     \___|    |   |    |  / /        \|  |  / __ \|  |  
 \______  /____|   |______/ /_______  /|__| (____  /__|  
        \/                          \/           \/      

*/
void DisplayCPUStat () {

  //--------------------------------------- Display Background ----------------------------------------------------

  /* OLED Background */
  display.setTextSize(2); //set background txt font size
  display.setCursor(1, 12);
  display.println("CPU");

  /*CPU Clearing Boxes, eg: display.fillRect(<X>, ^Y^, W, H, Color);*/
  display.fillRect(38, 11, 90, 16, BLACK); // fill CPU live stats section only
  
  /* Position Test*/
  //display.drawRect(38, 11, 90, 16, WHITE); // fill CPU live stats section only
  //display.drawRect(38, 37, 90, 16, WHITE); // fill GPU live stats section only
  //display.drawRect(38, 54, 36, 10, WHITE); // fill RAM live stats section only
  //display.drawRect(68, 54, 58, 10, WHITE); // Flash bottom right corner when updating

  //---------------------------------------CPU & GPU Hardware ID----------------------------------------------------

  /*CPU Hardware ID*/

  /*CPU Manual Position Test*/
  //display.setTextSize(0);
  //display.setCursor(-35, 1);// Negative spacing so, CPU ID doesn't cause a rollover, on the next line
  //String cpuName = "Intel Core i7-5820K"; // Name spacing test

  //display.setTextSize(0); // string font size
  //String cpuName = "";
  //display.println(cpuName); 

  if (inputString.indexOf("CPU") > -1)
  {
    String cpuName = "";
    display.setTextSize(1);
    display.setCursor(0, 1);

    int cpuNameStart = inputString.indexOf("CPU:");
    cpuName = inputString.substring(cpuNameStart + 4, inputString.indexOf("GPU:"));
    display.println(cpuName);

  }

  //------------------------------------------ CPU Load/Temp -------------------------------------------------

  /*CPU Display String*/
  int cpuStringStart = inputString.indexOf("|CT");
  int cpuDegree = inputString.indexOf("ct");
  int cpuStringLimit = inputString.indexOf("|G");
  String cpuString1 = inputString.substring(cpuStringStart + 3, cpuDegree);
  String cpuString2 = inputString.substring(cpuDegree + 2, cpuStringLimit);

  /*CPU TEMPERATURE*/
  display.setTextSize(2);
  display.setCursor(42, 12);
  display.print(cpuString1);
  display.setTextSize(1);

#ifdef noDegree
  display.print("C");       // Centigrade Symbol
#else
  display.print((char)247); //Degrees Symbol
  display.print("C");       // Centigrade Symbol
#endif

  /*CPU LOAD, ALL CORES*/
  display.setTextSize(2);
  display.print(cpuString2);
  display.setTextSize(1);
  display.println("%");    // Small Percent Symbol

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
