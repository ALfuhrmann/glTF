// Copyright (c) 2012, Motorola Mobility, Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the Motorola Mobility, Inc. nor the names of its
//    contributors may be used to endorse or promote products derived from this
//    software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "GLTF.h"

using namespace rapidjson;

namespace GLTF
{
    GLTFEffect::GLTFEffect(const std::string& ID):
    _ID(ID) {
    }

    GLTFEffect::~GLTFEffect() {
    }
                        
    const std::string& GLTFEffect::getID() {
        return this->_ID;
    }
    
    void GLTFEffect::setTechniqueID(const std::string& techniqueID) {
        this->_techniqueID = techniqueID;
    }
    
    const std::string& GLTFEffect::getTechniqueID() {
        return this->_techniqueID;
    }
    
    void GLTFEffect::setName(const std::string& name) {
        this->_name = name;
    }
    
    const std::string& GLTFEffect::getName() {
        return this->_name;
    }
    
    void GLTFEffect::setValues(shared_ptr <JSONObject> values) {
        this->_values = values;
    }
    
    shared_ptr <JSONObject> GLTFEffect::getValues() {
        return this->_values;
    }
    
    void GLTFEffect::setLightingModel(const std::string& lightingModel) {
        this->_lightingModel = lightingModel;
    }
    
    const std::string& GLTFEffect::getLightingModel() {
        return this->_lightingModel;
    }
    
    void GLTFEffect::addSemanticForTexcoordName(const std::string &texcoord, const std::string &semantic) {
        shared_ptr <SemanticArray> semanticArrayPtr;
        if (this->_texcoordToSemantics.count(texcoord) == 0) {
            semanticArrayPtr =  shared_ptr<SemanticArray> (new SemanticArray());
            this->_texcoordToSemantics[texcoord] = semanticArrayPtr;
        } else {
            semanticArrayPtr = this->_texcoordToSemantics[texcoord];
        }
        
        semanticArrayPtr->push_back(semantic);
    }
    
    SemanticArrayPtr GLTFEffect::getSemanticsForTexcoordName(const std::string &texcoord) {
        return this->_texcoordToSemantics[texcoord];
    }

}
