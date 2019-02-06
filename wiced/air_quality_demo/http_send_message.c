#include "wiced.h"
#include "http_client.h"

#include "http_send_message.h"

#include "air_quality_demo_secrets.h"

/*
 * This is based on https://www.cypress.com/blog/technical/wiced-http-client
 */



#define SERVER_PORT (80)
#define SERVER_HOST "demo.thingsboard.io"
#define SERVER_RESOURCE "/api/v1/" THINGBOARD_DEVICE_TOKEN "/telemetry"
#define DNS_TIMEOUT_MS ( 10000 )
#define CONNECT_TIMEOUT_MS ( 3000 )
wiced_semaphore_t requestSemaphore; // used to signal request is done static http_client_t client;
static http_request_t request;
static http_client_configuration_info_t client_configuration;
static wiced_ip_address_t ip_address;
static http_client_t  client;


wiced_result_t init_http()
{
    wiced_result_t result = wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);
    if (result != WICED_SUCCESS) {
        return result;
    }
    return wiced_hostname_lookup(SERVER_HOST, &ip_address, DNS_TIMEOUT_MS, WICED_STA_INTERFACE);
}


void send_message_http(float temp, float rh, float tvoc, float co2eq, float pm1_0, float pm2_5, float pm4_0, float pm10_0)
{

    http_client_init( &client, WICED_STA_INTERFACE, event_handler, NULL );

    /* configure HTTP client parameters */
    client_configuration.flag = (http_client_configuration_flags_t)(HTTP_CLIENT_CONFIG_FLAG_SERVER_NAME | HTTP_CLIENT_CONFIG_FLAG_MAX_FRAGMENT_LEN);
    client_configuration.server_name = (uint8_t*)SERVER_HOST;
    client_configuration.max_fragment_length = TLS_FRAGMENT_LENGTH_1024;
    http_client_configure(&client, &client_configuration);

    http_client_connect( &client, (const wiced_ip_address_t*)&ip_address, SERVER_PORT, 0, CONNECT_TIMEOUT_MS );

    http_header_field_t header[3]; // Three headers
    char messageBody[255];        // Enough to hold the message body
    char messageLengthBuffer[10]; // Enough to hold the characters for the Content-Length: header

    sprintf(messageBody,"{temp:%f,rh:%f,tvoc:%f,co2eq:%f,pm1_0:%f,pm2_5:%f,pm4_0:%f,pm10_0:%f}",
                        temp, rh, tvoc, co2eq, pm1_0, pm2_5, pm4_0, pm10_0);

    header[0].field        = HTTP_HEADER_HOST;
    header[0].field_length = sizeof( HTTP_HEADER_HOST ) - 1;
    header[0].value        = SERVER_HOST;
    header[0].value_length = sizeof( SERVER_HOST ) - 1;

#define MIME_FORM_URL "application/json"
    header[1].field        = HTTP_HEADER_CONTENT_TYPE;
    header[1].field_length = sizeof( HTTP_HEADER_CONTENT_TYPE ) - 1;
    header[1].value        =  MIME_FORM_URL;
    header[1].value_length = sizeof(  MIME_FORM_URL ) - 1;

    sprintf(messageLengthBuffer," %d",strlen(messageBody)); // Put the message body into the buffer so that you can strlen it
    header[2].field        = HTTP_HEADER_CONTENT_LENGTH;
    header[2].field_length = sizeof( HTTP_HEADER_CONTENT_LENGTH ) - 1;
    header[2].value        =  messageLengthBuffer;
    header[2].value_length = strlen(messageLengthBuffer);

    // Build the HTTP Message

    http_request_init( &request, &client, HTTP_POST, SERVER_RESOURCE, HTTP_1_1 );
    http_request_write_header( &request, &header[0], 3 ); // 3 headers
    http_request_write_end_header( &request );
    http_request_write(&request,(uint8_t*)messageBody,strlen(messageBody));
    http_request_flush( &request );

    wiced_rtos_get_semaphore(&requestSemaphore,10000); // wait up to 10 seconds to close the request and the client
    http_request_deinit(&request);
    http_client_deinit(&client);
}

static void print_data( char* data, uint32_t length )
{
    uint32_t a;

    for ( a = 0; a < length; a++ )
    {
        WPRINT_APP_INFO( ( "%c", data[a] ) );
    }
}

static void print_content( char* data, uint32_t length )
{
    WPRINT_APP_INFO(( "==============================================\n" ));
    print_data( (char*)data, length );
    WPRINT_APP_INFO(( "\n==============================================\n" ));
}

void event_handler( http_client_t* client, http_event_t event, http_response_t* response )
{
    switch( event )
    {
        case HTTP_CONNECTED: // Dont do anything
        break;

        case HTTP_DISCONNECTED: // Disconnect if you get this event
            wiced_rtos_set_semaphore(&requestSemaphore);
        break;

        case HTTP_DATA_RECEIVED: // Disconnect if you get this event
            WPRINT_APP_INFO( ( "\nRecieved response for request #1. Content received:\n" ) );

           /* print only HTTP header */
           if(response->response_hdr != NULL)
           {
               WPRINT_APP_INFO( ( "\n HTTP Header Information for response1 : \n" ) );
               print_content( (char*) response->response_hdr, response->response_hdr_length );
           }

            wiced_rtos_set_semaphore(&requestSemaphore);
        break;

        default:
        break;
    }
}
