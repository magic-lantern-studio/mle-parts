/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dgeomm.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Geometry Media Reference
 * targeting the BRender platform.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>

#ifdef WIN32
#include <io.h>
#include <windows.h>
#else //not WIN32
#include <unistd.h>
#endif

// Include Magic Lantern header files.
#include "math/scalarbr.h"
#include "math/vector.h"
#include "mle/3dgeomm.h"
#include "mle/brendapi.h"


MLE_MEDIAREF_SOURCE(Mle3dGeometryMedia,MleMediaRef);


Mle3dGeometryMedia::Mle3dGeometryMedia()
{
    //buffer = NULL;
    m_geometryCount = 0;
    m_geometryList = NULL;
}


Mle3dGeometryMedia::~Mle3dGeometryMedia()
{
    // Do nothing.
}


// XXX - This is a hokey max BRender needs to load registries
#define MLE_MAX_MODELS 1024  


br_model** Mle3dGeometryMedia::read()
// Gets all the models from the member buffer and parents them all to 
// the actor which is returned.
{
    // Declare local variables.
    char *buffer = m_references->buffer;

    // Read BRender Model Registry from external reference
    if (buffer) 
        m_geometryList = loadModelRegistry((char*)buffer,m_geometryCount);

    if (! (m_geometryList && m_geometryCount))
        fprintf(stderr,"Mle3dGeometryMedia::Warning--no geometry found in geometry registry.\n");

    return m_geometryList;
}


br_model** Mle3dGeometryMedia::loadModelRegistry(char* filename, unsigned int& numModels)
// Loads a model registry from the given file and returns a list of models.  
// Also sets the number of models read in the numModels parameter
{ 
    br_model** modelList = NULL;
    numModels = 0;
    br_model* tempList[MLE_MAX_MODELS];

    if (filename)
	{ 
        int filecheck = open(filename,O_RDONLY);
        if (filecheck>-1)
		{
			close(filecheck);

            int tempCount=BrModelLoadMany(filename,tempList,MLE_MAX_MODELS);
            if (tempCount)
			{
				numModels = tempCount;
                modelList = new br_model*[numModels];
                for (unsigned int i = 0; i < numModels; i++)
                    modelList[i] = tempList[i];
			}
            else
                fprintf(stderr,"Mle3dGeometryMedia::Error--Could not load geometry from file %s.\n",filename);
		}
        else
            fprintf(stderr,"Mle3dGeometryMedia::Error--Could not find geometry registry file %s.\n",filename);
	} 
    else
        fprintf(stderr,"Mle3dGeometryMedia::Error--No filename given for geometry registry.\n");


    return modelList;
}


br_actor* mlBrLoadModelRegistry(char* filename, char* id)
{
    unsigned int numModels;
    br_model** registry = Mle3dGeometryMedia::loadModelRegistry(filename,numModels); 
    if (numModels)
        return mlBrGetAllModels(registry, numModels, id);
    else return NULL;
}


br_actor* mlBrGetAllModels(br_model** geometryList, unsigned int numModels, char* id)
{
	// Creates actors from all the given models in the geometryList,
    // and makes them all children of a parent actor which is returned.
    // XXX Uses the string token after the newline in the identifier field 
    // to match up the models with materials from the materials registry,
    char* string;
    char* token;

    br_actor* parent = BrActorAllocate(BR_ACTOR_NONE,NULL);
    parent->t.type = BR_TRANSFORM_MATRIX34;

    // The actor is assigned the name given by the input id
    parent->identifier=strdup(id);

    if (numModels)
        for (unsigned int i = 0; i < numModels; i++)
		{
			string = strdup(geometryList[i]->identifier);
            token = strtok(string,"\n");
            br_actor* myActor = BrActorAllocate(BR_ACTOR_MODEL,NULL);
            myActor->identifier = strdup(id);
            myActor->t.type = BR_TRANSFORM_MATRIX34;
            myActor->model = BrModelAdd(geometryList[i]);
            token=strtok(NULL,"");
            myActor->material = BrMaterialFind(token);
            BrActorAdd(parent,myActor);
		}
    return parent;
}


br_actor* mlBrGetModelsWithName(char* name, br_model** geometryList, unsigned int numModels, char* id)
{
    // Creates actors from the models in the given geometryList which have an 
    // identifier matching the given name and makes them all children of a 
    // parent actor which is returned.
    // XXX - Uses the next token after the newline in thes identity field to
    // match up the models with materials from the global materials registry
    char* string;
    char* token;

    br_actor* actor = BrActorAllocate(BR_ACTOR_NONE,NULL); 
    actor->t.type = BR_TRANSFORM_MATRIX34;

    // The actor is assigned the name given by the input id
    actor->identifier = strdup(id);
 
    int length=strlen(name);

    if (length && geometryList)
        for (unsigned int i = 0; i < numModels; i++)
             if (strncmp(name,geometryList[i]->identifier,length) == 0)
             {
				 string = strdup(geometryList[i]->identifier);
                 token=strtok(string,"\n");
                 br_actor* myActor=BrActorAllocate(BR_ACTOR_MODEL,NULL);
                 myActor->identifier=strdup(id);
                 myActor->t.type = BR_TRANSFORM_MATRIX34;
                 myActor->model = BrModelAdd(geometryList[i]);
                 token=strtok(NULL,"");
                 myActor->material = BrMaterialFind(token);
                 BrActorAdd(actor,myActor);
             }

    return actor;
}


br_actor** mlBrGetModelGroups(br_model** geometryList, unsigned int numModels, unsigned int& numActors)
{
	// Searches the geometryList for models with matching identifiers and
    // groups each matching set together in one actor.  Returns a list 
    // of all the actor groups with matching names.
    // XXX Uses the next token after the newline in thes identity field to
    // match up the models with materials from the global materials registry
    char* string;
    char* token;
 
    br_actor** actorList = NULL;
    numActors = 0;
    int found;

    if (geometryList && numModels)
	{
		actorList = new br_actor*[numModels];
        for (unsigned int i = 0; i < numModels; i++)
		{
			found = FALSE;
            for (unsigned int j = 0; j < numActors; j++)
                // Group name just has to match first token in identifier
                if (strncmp(geometryList[i]->identifier,actorList[j]->identifier,
                            strlen(actorList[j]->identifier)) == 0)
				{
					found = TRUE; 
                    // Found a matching actor--add actor this one.
                    br_actor* myActor = BrActorAllocate(BR_ACTOR_MODEL,NULL);
                    // The actor will get the name of the parent group
                    myActor->identifier = strdup(actorList[j]->identifier);
                    myActor->t.type = BR_TRANSFORM_MATRIX34;
                    myActor->model = BrModelAdd(geometryList[i]);
                    string = strdup(geometryList[i]->identifier);
                    token = strtok(string,"\n");
                    // XXX The second half of the model name is the material name
                    token = strtok(NULL,"");
                    myActor->material = BrMaterialFind(token);
                    BrActorAdd(actorList[j],myActor);
                    // now break out of this loop
                    j = numActors;
				}
                if (! found)
				{
					// No matching actor--create a new one.
                    actorList[numActors] = BrActorAllocate(BR_ACTOR_MODEL,NULL);
                    string = strdup(geometryList[i]->identifier);
                    token = strtok(string,"\n");
                    // The parent actor will have the name that matches all groups
                    actorList[numActors]->identifier = strdup(token);
                    actorList[numActors]->t.type = BR_TRANSFORM_MATRIX34;
                    actorList[numActors]->model = BrModelAdd(geometryList[i]);
                    // XXX The second half of the model name is the material name
                    token = strtok(NULL,"");
                    actorList[numActors]->material = BrMaterialFind(token);
                    numActors++;
				}
		}
	}
    return actorList;
}


br_model* mlBrGetModel(char* filename)
// Gets a model from the global model registry or loads if from a file if 
// it does not exist in the registry
{
	br_model* myModel = NULL;

    if ((myModel = BrModelFind(filename)) == NULL)
	{
        if ((myModel = BrModelLoad(filename)) != NULL)
		{
			myModel->identifier = strdup(filename);
            BrModelAdd(myModel);
        }
	}

    return myModel;
}


br_model* mlBrGetModel(char* filename, unsigned int numPaths, char** pathList)
// Gets a model from the global model registry or loads if from a file 
// in the given file path list if the model does not exist in the registry, 
{
	br_model* myModel = NULL;
    unsigned int found = 0;
    char filepath[1024];

    if (filename != NULL)
	{
		if ((myModel = BrModelFind(filename))!=NULL)
            found++;
        else
        if ((pathList != NULL) && (numPaths > 0))
		{
			unsigned int i;
            for (i = 0; (i < numPaths) && (! found); i++)
			{ 
                if (pathList[i] != NULL)
				{
					filepath[0] = 0;
                    strcpy(filepath,pathList[i]);
                    strcat(filepath,"\\");
                    strcat(filepath,filename);
				}
                else
				{
					filepath[0] = 0;
                    strcpy(filepath,filename);
				}

                if ((myModel = BrModelFind(filepath))!=NULL)
                   found++;
			}
            if (! found)
                if ((myModel = BrModelLoad(filename))!=NULL)
				{
					found++;
                    myModel->identifier = strdup(filename);
                    BrModelAdd(myModel);
				}
            for (i=0;(i<numPaths)&&(!found);i++)
			{ 
                if (pathList[i] != NULL)
				{
					filepath[0] = 0;
                    strcpy(filepath,pathList[i]);
                    strcat(filepath,"/");
                    strcat(filepath,filename);
				}
                else
				{
					filepath[0] = 0;
                    strcpy(filepath,filename);
				}

                if ((myModel = BrModelLoad(filepath))!=NULL)
				{
					found++;
                    myModel->identifier=strdup(filepath);
                    BrModelAdd(myModel);
				}
			}
		}
        else
            myModel=mlBrGetModel(filename);
	}

    return myModel;
}



unsigned long ml3dGetAnimationIdentifier(char* name)
// Possibly use this field at a later date to uniquely identify animations
{
	unsigned long result = 0;
    int length = strlen(name);

    for (int i = 0; i < length; i++)
        result += (length - i + 1) * name[i] * name[i] + name[i];
    return result;
}


br_actor* mlBrGetModelsWithIdentifier(unsigned long target, br_model** geometryList, int numModels, char* id)
{
    // Creates actors from the models in the given geometryList which have an 
    // identifier matching the given name and makes them all children of a 
    // parent actor which is returned.
    // XXX Uses the next token after the newline in thes identity field to
    // match up the models with materials from the global materials registry
    char* string;
    char* token;

    br_actor* actor = BrActorAllocate(BR_ACTOR_NONE,NULL); 
    actor->t.type = BR_TRANSFORM_MATRIX34;

    // The actor is assigned the name given by the input id
    actor->identifier = strdup(id);
 
    if (geometryList)
        for (int i = 0; i < numModels; i++)
		{
			string = strdup(geometryList[i]->identifier);
            token = strtok(string,"\n");
            // The animation identifier is a number but the geometry identifier is a string
            if (target == (unsigned long)atoi(token))
			{
				string = strdup(geometryList[i]->identifier);
                token = strtok(string,"\n");
                br_actor* myActor = BrActorAllocate(BR_ACTOR_MODEL,NULL);
                myActor->identifier = strdup(id);
                myActor->t.type = BR_TRANSFORM_MATRIX34;
                myActor->model = BrModelAdd(geometryList[i]);
                token = strtok(NULL,"");
                myActor->material = BrMaterialFind(token);
                BrActorAdd(actor,myActor);
			}
		}
    return actor;
}

