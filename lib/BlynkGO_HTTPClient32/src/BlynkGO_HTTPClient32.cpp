#include "BlynkGO_HTTPClient32.h"
//#include "LCBUrl.h"
#include <BeeNeXT.h>


static const char *TAG = "BlynkGO_HTTPClient32";

static void* IRAM_ATTR esp32_malloc(size_t size) { 
  void* buf;
#if defined (ESP32) && defined (CONFIG_SPIRAM_SUPPORT)
  if ( psramFound() ) 
     buf = ps_malloc(size);
  else
#endif
  buf = malloc(size);

  if(buf !=NULL) memset(buf,0, size);
  return buf;
}

static esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
  static char *output_buffer;  // Buffer to store response of http request from event handler
  static int output_len;       // Stores number of bytes read

  BlynkGO_HTTPClient32* pHTTPClient = (BlynkGO_HTTPClient32*) evt->user_data;
  switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
//      Serial.println("[BlynkGO_HTTPClient32] event : HTTP_EVENT_ERROR");
      break;
    case HTTP_EVENT_ON_CONNECTED:
      // Serial.println("[BlynkGO_HTTPClient32] event : HTTP_EVENT_ON_CONNECTED");
      // pHTTPClient->_http_checking = false;
      output_len = 0;
      pHTTPClient->_is_connected = true;
      if( pHTTPClient->_fn_on_connected != NULL){
        pHTTPClient->_fn_on_connected();
      }
      break;
    case HTTP_EVENT_HEADER_SENT:
//      Serial.println("[BlynkGO_HTTPClient32] event : HTTP_EVENT_HEADER_SENT");
      break;
    case HTTP_EVENT_ON_HEADER:
//      Serial.printf("[BlynkGO_HTTPClient32] event : HTTP_EVENT_ON_HEADER, key=%s, value=%s\n", evt->header_key, evt->header_value);
      if( pHTTPClient->_fn_on_header_cb != NULL){
        pHTTPClient->_fn_on_header_cb( evt->header_key, evt->header_value);
      }
      break;
    case HTTP_EVENT_ON_DATA:
//      Serial.printf("[BlynkGO_HTTPClient32] event : HTTP_EVENT_ON_DATA, len=%d\n", evt->data_len);
      if( pHTTPClient->_fn_on_data != NULL){
        output_len += evt->data_len;
        pHTTPClient->_fn_on_data( (uint8_t*)evt->data, evt->data_len, output_len >=pHTTPClient-> contentLength() );
        return ESP_OK;
      }

      /*
          Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
          However, event handler can also be used in case chunked encoding is used.
      */
      if (!esp_http_client_is_chunked_response(evt->client)) {
        // If user_data buffer is configured, copy the response into the buffer
        if (output_buffer == NULL) {
          output_buffer = (char *) esp32_malloc(pHTTPClient->contentLength() + 1);
          output_len = 0;
          if (output_buffer == NULL) {
            Serial.println("[BlynkGO_HTTPClient32] Failed to allocate memory for output buffer");
            return ESP_FAIL;
          }
        }
        memcpy(output_buffer + output_len, evt->data, evt->data_len);
        output_len += evt->data_len;
      }

      break;
    case HTTP_EVENT_ON_FINISH:
//      Serial.println("[BlynkGO_HTTPClient32] event : HTTP_EVENT_ON_FINISH");

      if (output_buffer != NULL) {
        if( pHTTPClient->_fn_on_response_cb != NULL){
          pHTTPClient->_fn_on_response_cb( pHTTPClient->httpCode(), (uint8_t*) output_buffer, pHTTPClient->contentLength() );
        }
        // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
        // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
        free(output_buffer);
        output_buffer = NULL;
      }
      output_len = 0;
      break;
    case HTTP_EVENT_DISCONNECTED:
//      Serial.println("[BlynkGO_HTTPClient32] event : HTTP_EVENT_DISCONNECTED");
      pHTTPClient->_is_connected = false;
      int mbedtls_err = 0;
      esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
      if (err != 0) {
//        Serial.printf("Last esp error code: 0x%x\n", err);
//        Serial.printf("Last mbedtls failure: 0x%x\n", mbedtls_err);
      }
      if (output_buffer != NULL) {
        free(output_buffer);
        output_buffer = NULL;
      }
      output_len = 0;

      if( pHTTPClient->_fn_on_disconnected != NULL){
        pHTTPClient->_fn_on_disconnected();
      }

      break;
    // case HTTP_EVENT_REDIRECT:
    //   Serial.println("HTTP_EVENT_REDIRECT");
    //   esp_http_client_set_header(evt->client, "From", "user@example.com");
    //   esp_http_client_set_header(evt->client, "Accept", "text/html");
    //   break;
  }
  return ESP_OK;
}

BlynkGO_HTTPClient32& BlynkGO_HTTPClient32::begin(String url, const char* cert_pem){
  if(this->_client != NULL)  this->end();   // หากมีการเริ่มใหม่ ให้ทำลายเดิมๆ คืนเม็มให้สะอาดๆก่อน

  _url = url;
  esp_http_client_config_t config = {};
  {
    config.url                    =  (const char*) _url.c_str();
    config.cert_pem               =  (const char*) cert_pem;    // แบบ insecure ต้องใส่บรรทัดนี้ เป็น NULL ไม่งั้นเชื่อมไม่ได้
    config.event_handler          = _http_event_handler;
    config.user_data              = this;        // Pass address of local buffer to get response
    config.buffer_size            = MAX_HTTP_RECV_BUFFER;
    config.buffer_size_tx         = MAX_HTTP_OUTPUT_BUFFER;
    config.disable_auto_redirect  = true;
    config.timeout_ms             = 5000;
    if(_url.startsWith("https") || _url.startsWith("HTTPS")) config.is_async = true;
  }
  _client = esp_http_client_init(&config);
  _is_connected = false;
  return *this;
}

void BlynkGO_HTTPClient32::end(){
  if(this->_is_connected) {
    this->_http_checking = false;  // เพื่อให้ใน task ที่เคยทำงานอยู่ จะไปทำลายตัวเองลงไป
    // esp_http_client_close(_client);
    esp_http_client_cleanup(_client);
  }
  this->_is_connected  = false;
}

BlynkGO_HTTPClient32& BlynkGO_HTTPClient32::GET(){
  // GET
  esp_err_t err = esp_http_client_perform(_client);
  if (err == ESP_OK) {
//    Serial.printf( "[BlynkGO_HTTPClient32] HTTP GET Status = %d, content_length = %d\n",
//                                        this->httpCode(), this->contentLength() );
//                   esp_http_client_get_status_code(_client),
//                   esp_http_client_get_content_length(_client));
  } else {
//    Serial.printf("[BlynkGO_HTTPClient32] HTTP GET request failed: %s\n", esp_err_to_name(err));
  }
  //  ESP_LOG_BUFFER_HEX(TAG, local_response_buffer, strlen(local_response_buffer));
//  Serial.println(this->response_buffer);
//  esp_http_client_close  (_client);
  // esp_http_client_cleanup(_client);
    
  return *this;
}

BlynkGO_HTTPClient32& BlynkGO_HTTPClient32::POST(String post_data){
  // POST
  esp_http_client_set_method(_client, HTTP_METHOD_POST);
  if(post_data != "") {
    this->_post_data = post_data;  // เพื่อให้ข้อมูลยังค้างอยุ่ ให้มาจำไว้ส่วนตัวของ class instant
    esp_http_client_set_post_field(_client, this->_post_data.c_str(), this->_post_data.length());
  }
  this->_http_checking = true;
  xTaskCreatePinnedToCore( [](void* arg){
    BlynkGO_HTTPClient32* pHTTPClient = (BlynkGO_HTTPClient32*) arg;
    while(1){
      if(pHTTPClient->_http_checking) {
        esp_err_t err = esp_http_client_perform(pHTTPClient->_client);
        if( err != ESP_ERR_HTTP_EAGAIN) {
          pHTTPClient->_http_checking = false;
          if (err == ESP_OK) {
              ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %d",
                      esp_http_client_get_status_code   (pHTTPClient->_client),
                      esp_http_client_get_content_length(pHTTPClient->_client));
          } else {
              ESP_LOGI(TAG, "Error perform http request %s\n", esp_err_to_name(err));
          }
        } 
      }else{
        vTaskDelete(NULL);
      }
      vTaskDelay(1 / portTICK_PERIOD_MS);  
    }
  }, "BlynnkGO_HTTPClient_TASK", 4096, this, 5, &_task_handler, 0); 

  return *this;
}



// BlynkGO_HTTPClient32& BlynkGO_HTTPClient32::GET(String url, http_on_response_cb_t fn){
//   if(fn != NULL){ _fn_on_response_cb = fn; _fn_on_data = NULL; }
  
//   esp_http_client_config_t config = {};
//   {
//     config.url                    =  (const char*) url.c_str();
//     config.cert_pem               =  NULL;    // แบบ insecure ต้องใส่บรรทัดนี้ ไม่งั้นเชื่อมไม่ได้
//     config.event_handler          = _http_event_handler;
//     config.user_data              = this;        // Pass address of local buffer to get response
//     config.buffer_size            = MAX_HTTP_RECV_BUFFER;
//     config.buffer_size_tx         = MAX_HTTP_OUTPUT_BUFFER;
//     config.disable_auto_redirect  = true;
//   }
//   _client = esp_http_client_init(&config);

//   // GET
//   esp_err_t err = esp_http_client_perform(_client);
//   if (err == ESP_OK) {
// //    Serial.printf( "[BlynkGO_HTTPClient32] HTTP GET Status = %d, content_length = %d\n",
// //                                        this->httpCode(), this->contentLength() );
// //                   esp_http_client_get_status_code(_client),
// //                   esp_http_client_get_content_length(_client));
//   } else {
// //    Serial.printf("[BlynkGO_HTTPClient32] HTTP GET request failed: %s\n", esp_err_to_name(err));
//   }
//   //  ESP_LOG_BUFFER_HEX(TAG, local_response_buffer, strlen(local_response_buffer));
// //  Serial.println(this->response_buffer);
// //  esp_http_client_close  (_client);
//   esp_http_client_cleanup(_client);
    
//   return *this;
// }


int BlynkGO_HTTPClient32::httpCode(){
  return esp_http_client_get_status_code(_client);
}

int BlynkGO_HTTPClient32::contentLength(){
  return esp_http_client_get_content_length(_client);
}
