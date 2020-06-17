//
// Created by root on 20-3-15.
//
#include "sciter-common.h"
/**
 * init _GET _POST _REQUEST run_time var
 */
bool phpsciter::ZendSciterRequest::initRequest(const std::string& request_uri) {
    //initialize $_GET $_POST  $_REQUEST $_SERVER
    if (PG(auto_globals_jit)) {
#if PHP_VERSION_ID >= 70000
        zend_string *request_str = zend_string_init(ZEND_STRL("_REQUEST"), 0);
        zend_is_auto_global(request_str);
        zend_string_release(request_str);
#else
        if (PG(auto_globals_jit)) {
            zend_is_auto_global(ZEND_STRL(ZEND_REQUEST));
            zend_is_auto_global(ZEND_SERVER, sizeof(ZEND_SERVER)-1 TSRMLS_CC);
        }
#endif
    }

    if (PG(auto_globals_jit)) {
#if PHP_VERSION_ID >= 70000
        zend_string *request_str = zend_string_init(ZEND_STRL("_SERVER"), 0);
        zend_is_auto_global(request_str);
        zend_string_release(request_str);
#else
        zend_is_auto_global(ZEND_STRL(ZEND_SERVER));
#endif
    }

    //clear and init global var
#if PHP_VERSION_ID >= 70000
    zval array;
    zval* post_data = zend_hash_str_find(&EG(symbol_table),ZEND_POST,ZEND_POST_LEN);
    zval* get_data = zend_hash_str_find(&EG(symbol_table),ZEND_GET,ZEND_GET_LEN);
    zval* request_data = zend_hash_str_find(&EG(symbol_table),ZEND_REQUEST,ZEND_REQUEST_LEN);
    zval* server_data = zend_hash_str_find(&EG(symbol_table),ZEND_SERVER,ZEND_SERVER_LEN);
#else
    zval* array;
    int res;
    zval** post_data = nullptr;
    zval** get_data = nullptr;
    zval** request_data = nullptr;
    zval** server_data = nullptr;

    res = zend_hash_find(&EG(symbol_table), ZEND_STRS(ZEND_POST), (void**)&post_data);
    if(res == FAILURE)
    {
        post_data = nullptr;
    }
    res = zend_hash_find(&EG(symbol_table), ZEND_STRS(ZEND_GET), (void**)&get_data);
    if(res == FAILURE)
    {
        get_data = nullptr;
    }
    res =zend_hash_find(&EG(symbol_table), ZEND_STRS(ZEND_REQUEST), (void**)&request_data);
    if(res == FAILURE)
    {
        request_data = nullptr;
    }
    res = zend_hash_find(&EG(symbol_table), ZEND_STRS(ZEND_SERVER), (void**)&server_data);
    if(res == FAILURE)
    {
        server_data = nullptr;
    }
#endif

    if(get_data)
    {
#if PHP_VERSION_ID < 70000
        PHPSCITER_ZVAL_DTOR(*get_data);
        MAKE_STD_ZVAL(*get_data);
        array_init(*get_data);
#else
        PHPSCITER_ZVAL_DTOR(get_data);
        array_init(get_data);
#endif

    }else{
#if PHP_VERSION_ID >= 70000
        zval empty_get_data;
        array_init(&empty_get_data);
        get_data = zend_hash_str_add(&EG(symbol_table),ZEND_GET,ZEND_GET_LEN,&empty_get_data);
#else
        MAKE_STD_ZVAL(array);
        array_init(array);
        zend_hash_add(&EG(symbol_table), ZEND_GET, strlen(ZEND_GET)+1, (void *)&array, sizeof(zval*), (void**)&get_data);
#endif
    }
    if(post_data)
    {
#if PHP_VERSION_ID < 70000
        PHPSCITER_ZVAL_DTOR(*post_data);
        MAKE_STD_ZVAL(*post_data);
        array_init(*post_data);
#else
        PHPSCITER_ZVAL_DTOR(post_data);
        array_init(post_data);
#endif
    }else{
#if PHP_VERSION_ID >= 70000
        zval empty_post_data;
        array_init(&empty_post_data);
        post_data = zend_hash_str_add(&EG(symbol_table),ZEND_POST,ZEND_POST_LEN,&empty_post_data);
#else
        MAKE_STD_ZVAL(array);
        array_init(array);
        zend_hash_add(&EG(symbol_table), ZEND_POST, strlen(ZEND_POST)+1, (void *)&array, sizeof(zval*), (void**)&post_data);
#endif
    }
    if(request_data)
    {
#if PHP_VERSION_ID < 70000
        PHPSCITER_ZVAL_DTOR(*request_data);
        MAKE_STD_ZVAL(*request_data);
        array_init(*request_data);
#else
        PHPSCITER_ZVAL_DTOR(request_data);
        array_init(request_data);
#endif
    }else{
#if PHP_VERSION_ID >= 70000
        zval empty_request_data;
        array_init(&empty_request_data);
        request_data = zend_hash_str_add(&EG(symbol_table),ZEND_REQUEST,ZEND_REQUEST_LEN,&empty_request_data);
#else
        MAKE_STD_ZVAL(array);
        array_init(array);
        res = zend_hash_add(&EG(symbol_table), ZEND_REQUEST, strlen(ZEND_REQUEST)+1, (void *)&array, sizeof(zval*), (void**)&request_data);
#endif

    }
    if(!server_data)
    {
#if PHP_VERSION_ID >= 70000
        zval empty_server_data;
        array_init(&empty_server_data);
        array = PG(http_globals)[TRACK_VARS_SERVER];
        server_data = zend_hash_str_add(&EG(symbol_table),ZEND_SERVER,ZEND_SERVER_LEN,&empty_server_data);;
#else
        MAKE_STD_ZVAL(array);
//        array_init(array);
        zval_dtor(array);
        *array = *PG(http_globals)[TRACK_VARS_SERVER];
        INIT_PZVAL(array);
        zval_copy_ctor(array);
        res = zend_hash_add(&EG(symbol_table), ZEND_SERVER, strlen(ZEND_SERVER)+1, (void *)&array, sizeof(zval*), (void**)&server_data);
#endif
    }

    request_storage.request_uri = request_uri;
    request_storage.request_real_uri = request_uri;
#if PHP_VERSION_ID >= 70000
    request_storage.request_data = request_data;
    request_storage.get_data = get_data;
    request_storage.post_data = post_data;
    request_storage.server_data = server_data;
#else
    request_storage.request_data = *request_data;
    request_storage.get_data = *get_data;
    request_storage.post_data = *post_data;
    request_storage.server_data = *server_data;
#endif
    return true;
}

bool phpsciter::ZendSciterRequest::onRequest(LPSCN_LOAD_DATA load_data) {
    HREQUEST request_id;
    UINT request_param_number;
    uint8_t res;
    UINT request_param_count = 0;
    REQUEST_RQ_TYPE rq_type;

    //parse url info
    std::vector<string> url_vector;
    std::string parse_url(request_storage.request_uri);
    PHPSCITER_G(tool)->splitString(request_storage.request_uri,url_vector,"?");
    if(!url_vector.empty())
    {
        request_storage.request_real_uri = url_vector[0];
        if(url_vector.size() > 1)
        {
            std::vector<string> url_param_list;
            PHPSCITER_G(tool)->splitString(url_vector[1],url_param_list,"&");
            if(!url_param_list.empty())
            {
                std::vector<string> url_param_unit;
                size_t len = url_param_list.size();
                for(int i=0;i<len;i++)
                {
                    PHPSCITER_G(tool)->splitString(url_param_list[i],url_param_unit,"=");
                    if(!url_param_unit.empty())
                    {
                        size_t param_len = url_param_unit.size();
                        switch (param_len)
                        {
                            case 1:
                                request_storage.url_param[url_param_unit[0]] = "";
                                break;
                            case 2:
                                request_storage.url_param[url_param_unit[0]] = url_param_unit[1];
                                break;
                        }
                    }
                }
            }
        }
    }

    //parse request data
    request_id = load_data->requestId;
    res = rapi()->RequestGetRequestType(request_id, &rq_type);
    if (res != REQUEST_OK) {
        return false;
    }
    request_storage.request_type = rq_type;
    res = rapi()->RequestGetNumberOfParameters(request_id, &request_param_number);
    if (EXPECTED(res == REQUEST_OK))
    {
        if(request_param_number > 0) {
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
    } else{
        zend_error(E_WARNING, "RequestGetNumberOfParameters error");
        return false;
    }
    return true;
}

std::string& phpsciter::ZendSciterRequest::onComplete() {
    zval* storage_data;
    zval* request_data;
    map<std::string,std::string> request_info;
    map<std::string,std::string>::iterator request_map_iter;
    int res;

#if PHP_VERSION_ID>=70000
    zval zend_data;
    zval zend_request_uri;
#else
    zval* zend_data;
    zval* zend_request_uri;

    PHPSCITER_MAKE_STD_ZVAL(zend_request_uri);
#endif

    switch(request_storage.request_type)
    {
        case RRT_GET:
            storage_data = request_storage.get_data;
            break;
        case RRT_POST:
            storage_data = request_storage.post_data;
            break;
        default:
            return request_storage.request_real_uri;
            break;
    }
    request_data = request_storage.request_data;
    request_info = request_storage.request_table;

    if(!storage_data)
        return request_storage.request_real_uri;
    if(!request_data)
        return request_storage.request_real_uri;

    //parse url
#if PHP_VERSION_ID >= 70000
    PHPSCITER_ZVAL_STRING(&zend_request_uri,request_storage.request_uri.c_str());
    zend_hash_str_add(HASH_OF(request_storage.server_data),ZEND_SERVER_REQUEST_URI
            ,ZEND_SERVER_REQUEST_URI_LEN,&zend_request_uri);
#else
    PHPSCITER_ZVAL_STRING(zend_request_uri,request_storage.request_uri.c_str());
    res = zend_hash_add(HASH_OF(request_storage.server_data), ZEND_SERVER_REQUEST_URI, strlen(ZEND_SERVER_REQUEST_URI)+1
            , (void *)&zend_request_uri, sizeof(zval*), nullptr);
#endif


    for(request_map_iter = request_storage.url_param.begin();
        request_map_iter!=request_storage.url_param.end();request_map_iter++) {
        std::string key = request_map_iter->first;
        std::string value  = request_map_iter->second;
#if PHP_VERSION_ID >= 70000
        PHPSCITER_ZVAL_STRING(&zend_data, value.c_str());
        zend_hash_str_add(HASH_OF(request_storage.get_data),key.c_str(),key.length(),&zend_data);
#else
        PHPSCITER_MAKE_STD_ZVAL(zend_data);
        PHPSCITER_ZVAL_STRING(zend_data,value.c_str());
        res = zend_hash_add(HASH_OF(request_storage.get_data), key.c_str(), strlen(key.c_str())+1, (void *)&zend_data,
                      sizeof(zval*), nullptr);
#endif
#if PHP_VERSION_ID >= 70000
        PHPSCITER_ZVAL_STRING(&zend_data, value.c_str());
        zend_hash_str_add(HASH_OF(request_data),key.c_str(),key.length(),&zend_data);
#else
        PHPSCITER_MAKE_STD_ZVAL(zend_data);
        PHPSCITER_ZVAL_STRING(zend_data,value.c_str());
        res = zend_hash_add(HASH_OF(request_data), key.c_str(), strlen(key.c_str())+1, (void *)&zend_data,
                      sizeof(zval*), nullptr);
#endif
    }

    //parse data
    for(request_map_iter = request_info.begin();
    request_map_iter!=request_info.end();request_map_iter++) {
        std::string key = request_map_iter->first;
        std::string value  = request_map_iter->second;
#if PHP_VERSION_ID >= 70000
        PHPSCITER_ZVAL_STRING(&zend_data, value.c_str());
        zend_hash_str_add(HASH_OF(storage_data), key.c_str(),key.length(), &zend_data);
#else
        PHPSCITER_MAKE_STD_ZVAL(zend_data);
        PHPSCITER_ZVAL_STRING(zend_data,value.c_str());
        res = zend_hash_add(HASH_OF(storage_data), key.c_str(), strlen(key.c_str())+1, &zend_data,
                      sizeof(zval*), nullptr);
#endif

#if PHP_VERSION_ID >= 70000
        PHPSCITER_ZVAL_STRING(&zend_data, value.c_str());
        zend_hash_str_add(HASH_OF(request_data), key.c_str(),key.length(), &zend_data);
#else
        PHPSCITER_MAKE_STD_ZVAL(zend_data);
        PHPSCITER_ZVAL_STRING(zend_data,value.c_str());
        res = zend_hash_add(HASH_OF(request_data), key.c_str(), strlen(key.c_str())+1, &zend_data,
                      sizeof(zval*), nullptr);
#endif
    }

    return request_storage.request_real_uri;
}

std::string& phpsciter::ZendSciterRequest::getRequestUrl()
{
    return request_storage.request_real_uri;
}

void phpsciter::ZendSciterRequest::onClose()
{
    current_request_key.clear();
    current_request_value.clear();
    request_storage.request_real_uri.clear();
    request_storage.request_uri.clear();
    request_storage.url_param.clear();
    request_storage.server_data = nullptr;
    request_storage.post_data = nullptr;
    request_storage.get_data = nullptr;
    request_storage.request_data = nullptr;
    request_storage.request_type = RRT_GET;
    request_storage.request_table.clear();
}

