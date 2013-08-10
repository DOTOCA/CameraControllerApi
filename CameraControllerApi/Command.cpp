//
//  Command.cpp
//  CameraControllerApi
//
//  Created by Tobias Scheck on 09.08.13.
//  Copyright (c) 2013 scheck-media. All rights reserved.
//

#include "Command.h"
#include "Api.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>


using namespace CameraControllerApi;

struct validate_data {
    string api;
    set<string>* params;
};

Command::Command(){
    set<string> params;
    string param_camera_settings[] = {"aperture", "speed", "iso", "white_balance","focus_point","focus_mode"};
    string param_execute[] = {"shot","time_lapse","continuous_shooting"};
    _valid_commands["/settings"] = set<string>(param_camera_settings, param_camera_settings + 6);
    _valid_commands["/capture"] = set<string>(param_execute, param_execute + 3);
}

int Command::execute(const string &url, const map<string, string> &argvals, string &response){
    CCA_API_RESPONSE resp;
    CCA_API_OUTPUT_TYPE type = CCA_OUTPUT_TYPE_JSON;
    validate_data vdata;
    vdata.api = url;
    vector<string> params;
    set<string> uniqueparams;
    map<string,string>::const_iterator iterator = argvals.find("action");
    
    if(iterator != argvals.end()){
        string param = iterator->second;
        boost::trim(param);
        boost::split(params, param, boost::is_any_of(","));
    }
    
    BOOST_FOREACH(string pr, params ) {
        uniqueparams.insert(pr);
    }
    
    iterator = argvals.find("type");
    if(iterator != argvals.end()){
        const string out_type = iterator->second;
        if(strcasecmp(out_type.c_str(), "xml") == 0)
            type = CCA_OUTPUT_TYPE_XML;
    }

    vdata.params = &uniqueparams;
        
    if ( !this->_validate(&vdata) || uniqueparams.size() < 1) {
        resp = CCA_API_RESPONSE::invalid;
        Api::buildResponse(nullptr, type, resp, response);
        return resp;
    }
    
    
    return this->_executeAPI(url, uniqueparams, "xml", response);
}

bool Command::_executeAPI(const string &url, const set<string> &argvals, const char *type, string &response){
    bool ret = CCA_CMD_SUCCESS;
    return ret;
}

bool Command::_validate(const void *data){
    const validate_data *vdata = static_cast<const validate_data *>(data);
    map<string, set<string>>::iterator it = this->_valid_commands.find(vdata->api);
    
    if(it == this->_valid_commands.end()){
        return false;
    }
    
    set<string>::iterator param_it = vdata->params->begin();
    while(param_it != vdata->params->end()){
        if(it->second.find(*param_it) == it->second.end())
            return false;
        
        ++param_it;
    }
    
    return true;
}

void Command::_getInvalidResponse(string& response){
    response = "Some error in your data ";
}