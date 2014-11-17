/**
 * @file Switch.cpp
 *
 * Switch API of IoTgo (iotgo.iteadstudio.com)
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/11
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include "Switch.h"

const char *Switch::STATE[]        = {"state", NULL};
const char *Switch::VALUE_ON[]     = {"on",    NULL};
const char *Switch::VALUE_OFF[]    = {"off",   NULL};

Switch::Switch(uint16_t sw_pin)
{
    this->sw_pin = sw_pin;
    pinMode(this->sw_pin, OUTPUT);
}

/**
 * Read the state from server and turn on/off the switch. 
 * 
 * We always assume that the state from server is newest. 
 * 
 * @retval 0 - success
 * @retval SWITCH_ERR_NO_RESPONSE - no response package from server. 
 * @retval SWITCH_ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Switch::sync(void)
{
    int32_t state;
    int32_t ret = 0;
    state = getState();
    if (state == SWITCH_STATE_ON)
    {
        digitalWrite(sw_pin, HIGH);
    }
    else if (state == SWITCH_STATE_OFF)
    {
        digitalWrite(sw_pin, LOW);
    }
    else
    {
        DebugSerial.print("\ngetState() error: state = ");
        DebugSerial.println(state);
        ret = state;
    }
    return ret;
}

/**
 * Read state from server. 
 *
 * @retval SWITCH_STATE_ON - indicate the state is on. 
 * @retval SWITCH_STATE_OFF - indicate the state is off. 
 * @retval SWITCH_ERR_NO_RESPONSE - no response package from server. 
 * @retval SWITCH_ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Switch::getState(void)
{
    const char *response;
    char *str_error;
    char *str_params;
    char *str_state_on;
    char *str_state_off;
    
    response = query(STATE); /* {"error":0,"params":{"state":"on"}} {"error":0,"params":{"state":"off"}}*/
    if (response != NULL)
    {
        str_error       = strstr(response, "\"error\":0");
        str_params      = strstr(response, "\"params\":");
        str_state_on    = strstr(response, "\"state\":\"on\"");
        str_state_off   = strstr(response, "\"state\":\"off\"");
        
        if (str_error && str_params)
        {
            if (str_state_on && !str_state_off)
            {
                return SWITCH_STATE_ON;
            }
            else if (!str_state_on && str_state_off)
            {
                return SWITCH_STATE_OFF;
            }
            else
            {
                return SWITCH_ERR_NO_EXPECT;
            }
        }
        else
        {
            return SWITCH_ERR_NO_EXPECT;
        }
    }
    else
    {
        return SWITCH_ERR_NO_RESPONSE;
    }
}

/**
 * Update the specific state to server. 
 * 
 * @param state - the state you want to update, only SWITCH_STATE_ON or SWITCH_STATE_OFF. 
 * 
 * @retval 0 - success
 * @retval SWITCH_ERR_INVALID_PARAMETER - state is invalid value. 
 * @retval SWITCH_ERR_NO_RESPONSE - no response package from server. 
 * @retval SWITCH_ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Switch::setState(int32_t state)
{
    const char **value;
    const char *response;
    char *str_error;
    
    if (state == SWITCH_STATE_ON)
    {
        value = VALUE_ON;
    }
    else if (state == SWITCH_STATE_OFF)
    {
        value = VALUE_OFF;
    }
    else
    {
        return SWITCH_ERR_INVALID_PARAMETER;
    }
    
    response = update(STATE, value);
    if (response != NULL)
    {
        str_error = strstr(response, "\"error\":0");
        
        if (str_error)
        {
            return 0;
        }
        else
        {
            return SWITCH_ERR_NO_EXPECT;
        }
    }
    else
    {
        return SWITCH_ERR_NO_RESPONSE;
    }
}


/**
 * Turn on the switch and update the state to server. 
 *
 * @retval 0 - success
 * @retval SWITCH_ERR_NO_RESPONSE - no response package from server. 
 * @retval SWITCH_ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Switch::on(void)
{
    int32_t ret;
    ret = setState(SWITCH_STATE_ON) ;
    if (ret == 0)
    {
        digitalWrite(sw_pin, HIGH);        
    }
    return ret;
}

/**
 * Turn off the switch and update the state to server. 
 *
 * @retval 0 - success
 * @retval SWITCH_ERR_NO_RESPONSE - no response package from server. 
 * @retval SWITCH_ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Switch::off(void)
{
    int32_t ret;
    ret = setState(SWITCH_STATE_OFF) ;
    if (ret == 0)
    {
        digitalWrite(sw_pin, LOW);        
    }
    return ret;
}
