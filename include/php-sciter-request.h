//
// Created by root on 20-3-15.
//

#ifndef PHPSCITER_SCITERPARSE_H
#define PHPSCITER_SCITERPARSE_H

#include <memory>
#include <sciter-x-api.h>
#include <map>

using namespace std;

class ZendSciterRequest{

public:

    std::string current_request_key;

    std::string current_request_value;

    static inline VOID SC_CALLBACK callSciterRequestName(LPCWSTR key, UINT key_length, LPVOID param)
    {
        auto request = (ZendSciterRequest*)param;
        request->setRequestKey(key);
    }

    static inline VOID SC_CALLBACK callSciterRequestValue(LPCWSTR value, UINT value_length, LPVOID param)
    {
        auto request = (ZendSciterRequest*)param;
        request->setRequestValue(value);
    }

    BOOL setRequestKey(LPCWSTR key)
    {
        current_request_key = (aux::w2a(key).c_str());
        return TRUE;
    }

    BOOL setRequestValue(LPCWSTR value)
    {
        current_request_value = (aux::w2a(value).c_str());
        return TRUE;
    }

    //初始化请求
    BOOL initRequest(const std::string& request_uri);

    //请求到达的时候
    BOOL onRequest(LPSCN_LOAD_DATA load_data);

    //请求解析完成
    const std::string& onComplete();

    //clear memory
    void onClose();

private:
    //记录zend请求到达时候一些有用的数据
    typedef struct _zend_request_info{
        zval* get_data = nullptr;
        zval* post_data = nullptr;
        zval* request_data = nullptr;
        zval* server_data = nullptr;
        REQUEST_RQ_TYPE request_type = RRT_GET;
        std::string request_uri;
        std::string request_real_uri;
        map<std::string,std::string> url_param;
        map<std::string,std::string> request_table;
    }zend_request_info;

    zend_request_info request_storage;
};

#endif //PHPSCITER_SCITERPARSE_H
