#include "custom_service.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_l2cap.h"
#include "ble_srv_common.h"
#include "app_util.h"


#define OPCODE_LENGTH 1
#define HANDLE_LENGTH 2
//------------------------------------------------------------------------------
static uint8_t _status;
//------------------------------------------------------------------------------
static cs_cmd_t _commands[CS_CMD_NUM];
void CS_SetCommand(cs_cmd_t cmd, uint8_t cmdNum)
{
	_commands[cmdNum] = cmd;
}
static void (*_ledCB)(uint8_t*, uint8_t) = 0;
void CS_SetLEDCallback(void (*cb)(uint8_t*, uint8_t))
{
	_ledCB = cb;
}
//------------------------------------------------------------------------------
// Connect Event Handler
static void on_connect(ble_cs_t * p_cs, ble_evt_t * p_ble_evt)
{
    p_cs->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}
//------------------------------------------------------------------------------
// Disconnect Event Handler
static void on_disconnect(ble_cs_t * p_cs, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_cs->conn_handle = BLE_CONN_HANDLE_INVALID;
}
//------------------------------------------------------------------------------
// Writes to the Custom Service
//
// 
static void on_write(ble_cs_t * p_cs, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if (p_evt_write->handle == p_cs->command_handles.value_handle)
    {
        // Wrote to the command characteristic
        if (p_evt_write->len > 0 && p_evt_write->data[0] < CS_CMD_NUM)
		{
			if (_commands[p_evt_write->data[0]])
			{
				_commands[p_evt_write->data[0]](p_evt_write->data+1,p_evt_write->len-1); 
			}
		}
		
    }
}
//------------------------------------------------------------------------------
// BLE Event Handler
void ble_cs_on_ble_evt(ble_cs_t * p_cs, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_cs, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_cs, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_cs, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}
//============================================================================//
// COMMAND CHAR
// Add the command characteristic
static uint32_t _add_command_char(ble_cs_t* p_cs, const ble_cs_init_t* p_cs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
	//--------------------------------------------------------------------------
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    cccd_md.write_perm = p_cs_init->cs_command_attr_md.cccd_write_perm;
    cccd_md.vloc       = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
	char_md.char_props.write = 1;
	char_md.char_props.write_wo_resp = 1;
    char_md.p_char_user_desc  = (uint8_t*)"Command";
	char_md.char_user_desc_max_size = strlen("Command");
	char_md.char_user_desc_size = strlen("Command");
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = CS_UUID_TYPE;
    ble_uuid.uuid = CS_CHAR_COMMAND_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_cs_init->cs_command_attr_md.read_perm;
    attr_md.write_perm = p_cs_init->cs_command_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 23;
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_cs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_cs->command_handles);
}
//============================================================================//
// Add the Status characteristic
static uint32_t _add_status_char(ble_cs_t* p_cs, const ble_cs_init_t* p_cs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
	//--------------------------------------------------------------------------
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    cccd_md.write_perm = p_cs_init->cs_status_attr_md.cccd_write_perm;
    cccd_md.vloc       = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
	char_md.char_props.read = 1;

    char_md.p_char_user_desc  = (uint8_t*)"Status";
	char_md.char_user_desc_max_size = strlen("Status");
	char_md.char_user_desc_size = strlen("Status");
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = CS_UUID_TYPE;
    ble_uuid.uuid = CS_CHAR_STATUS_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_cs_init->cs_status_attr_md.read_perm;
    attr_md.write_perm = p_cs_init->cs_status_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 1;
    attr_char_value.p_value   = &_status;

    return sd_ble_gatts_characteristic_add(p_cs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_cs->status_handles);
}
//============================================================================//
uint32_t CS_Init(ble_cs_t * p_cs)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;
    ble_uuid.uuid = CUSTOM_SERVICE_UUID;
    ble_uuid.type = CS_UUID_TYPE;
	
    memset(_commands, 0, sizeof(cs_cmd_t)*CS_CMD_NUM);
	
    // Initialize service structure
    p_cs->evt_handler                 = NULL;
    p_cs->conn_handle                 = BLE_CONN_HANDLE_INVALID;

    // Add custom base UUID.
    ble_uuid128_t cs_base_uuid = {{CUSTOM_BASE_UUID}};
    err_code = sd_ble_uuid_vs_add(&cs_base_uuid, &ble_uuid.type);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    ble_uuid.uuid = CUSTOM_SERVICE_UUID;
    // Add service.
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_cs->service_handle);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    ble_cs_init_t cs_init;
    memset(&cs_init, 0, sizeof(cs_init));
    cs_init.evt_handler = NULL;
    // Security Levels for each characteristic
	//--------------------------------------------------------------------------
	// Command
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cs_init.cs_command_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cs_init.cs_command_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cs_init.cs_command_attr_md.write_perm);
	//--------------------------------------------------------------------------
	// STATUS
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cs_init.cs_status_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cs_init.cs_status_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&cs_init.cs_status_attr_md.write_perm);
	//--------------------------------------------------------------------------
    // Add command characteristic
    err_code = _add_command_char(p_cs, &cs_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
	// Add the status characteristic
    err_code = _add_status_char(p_cs, &cs_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    return NRF_SUCCESS;
}
//============================================================================//
uint32_t CS_SendStatusUpdate(ble_cs_t * p_cs, uint8_t status)
{
    uint32_t err_code;
    _status = status;
    // Send value if connected and notifying
    if (p_cs->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        uint16_t               hvx_len;
        ble_gatts_hvx_params_t hvx_params;

        hvx_len = 1;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_cs->status_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &hvx_len;
        hvx_params.p_data = &_status;

        err_code = sd_ble_gatts_hvx(p_cs->conn_handle, &hvx_params);
        if ((err_code == NRF_SUCCESS) && (hvx_len != 1))
        {
            err_code = NRF_ERROR_DATA_SIZE;
        }
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }

    return err_code;
}
