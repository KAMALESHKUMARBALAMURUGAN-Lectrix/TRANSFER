
{

  //       rx_task_action_t action;

  //     xQueueReceive(rx_task_queue, &action, portMAX_DELAY);
 
  xSemaphoreTake(ctrl_task_receive, portMAX_DELAY); // Wait for completion

  xSemaphoreGive(ctrl_task_send);                   // Start control task
 
  //  while (1) {
 
  //  vTaskDelay(pdMS_TO_TICKS(100));
 
  // if (action == RX_RECEIVE_COMPONENTS) {
 
  ESP_LOGI(EXAMPLE_TAG, "Reciving");

  //  vTaskDelay(pdMS_TO_TICKS(250));
 
  uint32_t MSG = 0;
 
 
  // Listen for ping response from slave

  while (1)

  {

    twai_message_t message;

    twai_receive(&message, pdMS_TO_TICKS(250));

    //           ESP_LOGI(EXAMPLE_TAG, "Reciving");
 
 
    if (MSG == message.identifier)

    {

      twai_clear_receive_queue();
 
      //printf("clearning RDX \n");

      //ESP_ERROR_CHECK(twai_stop());       


     // vTaskDelay(pdMS_TO_TICKS(1000));
 
 
     // ESP_ERROR_CHECK(twai_start());  
 
      MSG = 0;     
 
 
    }
 
        MSG = message.identifier;
 
 
//    ESP_LOGI(EXAMPLE_TAG, "message %lx",message.identifier);
 
 
  //    printf("mtr decode =  %x , %x , %x, %x , %x, %x, %x, %x  \n", message.data[0],message.data[1],message.data[2],message.data[3],message.data[4],message.data[5],message.data[6],message.data[7]);
 
 
    if (message.identifier == ID_MOTOR_RPM)

    { ///////    motor RPM

      // ESP_LOGI(EXAMPLE_TAG, " Motor RPM Data ");

      if (!(message.rtr))

      {
 
        // mcu_data = message.data;
 
        RPM = (message.data[1]) | (message.data[0] << 8);
 
            Motor_err = (message.data[7]) | (message.data[6]<<8) | (message.data[5]<<16)|(message.data[4]<<24) |(message.data[3]<<32)|(message.data[2]<<40) | (message.data[1]<<48)|(message.data[0]<<56);
 
            sprintf(motor_err, "%x,%x,%x,%x,%x,%x,%x,%x", message.data[0],message.data[1],message.data[2],message.data[3],message.data[4],message.data[5],message.data[6],message.data[7]);
 
 
        union

        {

          uint32_t b;

          int f;

        } u; // crazy

        u.b = RPM;
 
        // M_RPM = u.f;
 
         twai_message_t transmit_message = {.identifier = (0x14520902 ), .data_length_code = 8, .extd = 1, .data = {RPM, 0, 0, DC_VOLTAGE, DC_CURRENT}};

      if (twai_transmit(&transmit_message, 1000) == ESP_OK)

      {
 
        //          ESP_LOGI(EXAMPLE_TAG, "Message queued for transmission\n");

        //          vTaskDelay(pdMS_TO_TICKS(250));

      }

      else

      {
 
        ESP_LOGE(EXAMPLE_TAG, "Failed to queue message for transmission\n");

      }
 
      vTaskDelay(pdMS_TO_TICKS(50));

 
 
      }

    }
 
    else if (message.identifier == ID_MOTOR_TEMP)

    { /////        motor temp

      // ESP_LOGI(EXAMPLE_TAG, " Motor Temp Data ");

      if (!(message.rtr))

      {

        THROTTLE = (message.data[0]);
 
        CONT_TEMP = (message.data[1]);

        MOT_TEMP = (message.data[2]);

        union

        {

          uint32_t b;

          int f;

        } u; // crazy

        u.b = THROTTLE;

        M_THROTTLE = u.f;

        u.b = CONT_TEMP;

        M_CONT_TEMP = u.f - 40;

        u.b = MOT_TEMP;

        M_MOT_TEMP = u.f - 40;

        //  printf("Temp =  %d \n", M_CONT_TEMP);

      }

    }
 
    else if (message.identifier == ID_MOTOR_CURR_VOLT)

    {
 
      DC_CURRENT = (message.data[0]) | (message.data[1] << 8);

      AC_CURRENT = (message.data[4]) | (message.data[5] << 8);
 
      AC_VOLTAGE = (message.data[3]);
 
      DC_VOLTAGE = (message.data[2]);
 
      union

      {

        uint32_t b;

        int f;

      } u; // crazy

      u.b = DC_CURRENT;
 
      M_DC_CURRENT = u.f;
 
      u.b = AC_CURRENT;
 
      M_AC_CURRENT = u.f;
 
      u.b = AC_VOLTAGE;
 
      M_AC_VOLTAGE = u.f;
 
      u.b = DC_VOLTAGE;
 
      M_DC_VOLATGE = u.f;
 
    } /////        motor temp
 
 
else if (message.identifier == ID_LX_BATTERY_SOC)

    { ////          Battery SOC 3

      if (!(message.rtr))

      {

        SOC3_hx = (message.data[1] << 8) | message.data[0];

        SOH3_hx = message.data[5]  ;

        union

        {

          uint32_t b;

          int f;

        } u; // crazy

        u.b = SOC3_hx;

        SOC_3 = u.f;

        u.b = SOH3_hx;

        SOH_3 = u.f;

    //              printf("Battery 3 --> SOC[ %d ]   SOH[ %d ] \n", SOC_3,SOH_3);

      }

    }
 
else if (message.identifier == ID_LX_BATTERY_VI)

    { ////          Battery SOC 3

      if (!(message.rtr))

      {

       current2_hx  =  (message.data[0] << 24) | (message.data[1] << 16) | (message.data[2] << 8) | message.data[3];

        voltage2_hx =  (message.data[6] << 8) | (message.data[7] ) ;
 
    

       union

        {

          uint32_t b;

          int f;

        } u; // crazy

        u.b = voltage2_hx;

        Voltage_2 = u.f;

        u.b = current2_hx;

        Current_2 = u.f;

    //      printf("Battery 2 --> Volatge[%d]   current[%f]   \n", ( Voltage_2),(Current_2*0.001));
 
      }

    }
 
 
   else if (message.identifier == ID_LX_BATTERY_PROT)

    { ///////    motor RPM

      // ESP_LOGI(EXAMPLE_TAG, " Motor RPM Data ");

      if (!(message.rtr))

      {
 
        // mcu_data = message.data;
 
 
         //   BATT_ERR = (message.data[7]) | (message.data[6]<<8) | (message.data[5]<<16)|(message.data[4]<<24) |(message.data[3]<<32)|(message.data[2]<<40) | (message.data[1]<<48)|(message.data[0]<<56);
 
            sprintf(batt_err, "%x,%x,%x,%x,%x,%x,%x,%x", message.data[0],message.data[1],message.data[2],message.data[3],message.data[4],message.data[5],message.data[6],message.data[7]);
 
      }

    }
 
 
   else if (message.identifier == ID_LX_BATTERY_T)

    { ///////    motor RPM

      // ESP_LOGI(EXAMPLE_TAG, " Motor RPM Data ");

      if (!(message.rtr))

      {
 
        // mcu_data = message.data;
 
 
         //   BATT_ERR = (message.data[7]) | (message.data[6]<<8) | (message.data[5]<<16)|(message.data[4]<<24) |(message.data[3]<<32)|(message.data[2]<<40) | (message.data[1]<<48)|(message.data[0]<<56);
 
            sprintf(batt_temp, "%x,%x,%x,%x,%x,%x,%x,%x", message.data[0],message.data[1],message.data[2],message.data[3],message.data[4],message.data[5],message.data[6],message.data[7]);
 
      }

    }
 
 
    else

    {

      //   ESP_LOGE(EXAMPLE_TAG, " ID not match - %lx ",message.identifier );

      //   vTaskDelay(pdMS_TO_TICKS(250));

    }
 
    

       //   vTaskDelay(pdMS_TO_TICKS(50));
 
 
 
  }

  //   }
 
  //   else if (action == RX_TASK_EXIT) {

  //         break;

  //     }

  // }
 
  xSemaphoreGive(done_sem);
 
  vTaskDelete(NULL);

}
