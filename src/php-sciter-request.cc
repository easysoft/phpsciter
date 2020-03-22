//
// Created by root on 20-3-15.
//
#include "php_sciter.h"
/**
 * 初始化_GET _POST _REQUEST的运行时变量
 */
BOOL ZendSciterRequest::initRequest() {
    //初始化$_GET $_POST 和 $_REQUEST
    if (PG(auto_globals_jit)) {
        zend_string *request_str = zend_string_init(ZEND_STRL("_REQUEST"), 0);
        zend_is_auto_global(request_str);
        zend_string_release(request_str);
    }

    //清理全局变量运行时的内存
    zval array;
    array_init(&array);
    zval* post_data = zend_hash_str_find(&EG(symbol_table),ZEND_POST,ZEND_POST_LEN);
    zval* get_data = zend_hash_str_find(&EG(symbol_table),ZEND_GET,ZEND_GET_LEN);
    zval* request_data = zend_hash_str_find(&EG(symbol_table),ZEND_REQUEST,ZEND_REQUEST_LEN);
    if(get_data)
    {
        zval_ptr_dtor_nogc(get_data);
        array_init(get_data);
    }
    if(post_data)
    {
        zval_ptr_dtor_nogc(post_data);
        array_init(post_data);
    }
    if(request_data)
    {
        zval_ptr_dtor_nogc(request_data);
        array_init(request_data);
    }

    request_storage.request_data = request_data;
    request_storage.get_data = get_data;
    request_storage.post_data = post_data;
}

BOOL ZendSciterRequest::onRequest(LPSCN_LOAD_DATA load_data) {
    HREQUEST request_id;
    UINT request_param_number;
    uint8_t res;
    UINT request_param_count;
    REQUEST_RQ_TYPE rq_type;


    request_id = load_data->requestId;
    res = rapi()->RequestGetRequestType(request_id, &rq_type);
    if (res != REQUEST_OK) {
        return FALSE;
    }
    request_storage.request_type = rq_type;
    res = rapi()->RequestGetNumberOfParameters(request_id, &request_param_number);
    if (res == REQUEST_OK)
    {
        if(request_param_count > 0) {
            LPVOID
            request_ptr = PHPSCITER_G(request).get();
            for (request_param_count = 0; request_param_count < request_param_number; request_param_count++) {
                res = rapi()->RequestGetNthParameterName(request_id, request_param_count, callSciterRequestName,
                                                         request_ptr);
                res = rapi()->RequestGetNthParameterValue(request_id, request_param_count, callSciterRequestValue,
                                                          request_ptr);
                request_storage.request_table[current_request_key] = current_request_value;
            }
        }
    }
}

BOOL ZendSciterRequest::onComplete() {

    zval* storage_data;
    zval* request_data;
    map<std::string,std::string> request_info;
    map<std::string,std::string>::iterator request_map_iter;
    zval zend_data;

    switch(request_storage.request_type)
    {
        case RRT_GET:
            storage_data = request_storage.get_data;
            break;
        case RRT_POST:
            storage_data = request_storage.post_data;
            break;
        default:
            return FALSE;
            break;
    }
    request_data = request_storage.request_data;
    request_info = request_storage.request_table;

    if(!storage_data)
        return FALSE;
    if(!request_data)
        return FALSE;

    for(request_map_iter = request_info.begin();
    request_map_iter!=request_info.end();request_map_iter++) {
        std::string key = request_map_iter->first;
        std::string value  = request_map_iter->second;
        ZVAL_STRING(&zend_data,value.c_str());
        zend_hash_str_add(HASH_OF(storage_data),key.c_str(),key.length(),&zend_data);
        zend_hash_str_add(HASH_OF(request_data),key.c_str(),key.length(),&zend_data);
    }
}


