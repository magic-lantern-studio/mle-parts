/*
 ============================================================================
 Name        : exampleProgram.cpp
 Author      : Mark Millard
 Version     :
 Copyright   : (C) Wizzer Works 2020
 Description : Uses shared library to print greeting
               To run the resulting executable the LD_LIBRARY_PATH must be
               set to ${project_loc}/MleIvStage/.libs
               Alternatively, libtool creates a wrapper shell script in the
               build directory of this program which can be used to run it.
               Here the script will be called exampleProgram.
 ============================================================================
 */

// Include X11 header files.
#include <X11/IntrinsicP.h>

// Include Coin3D header files.
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtFullViewer.h>

// Include Magic Lantern header files.
#include "mle/ivstage.h"
#include "mle/MleDirector.h"
#include "mle/MleScheduler.h"

int main(void)
{
    // Initialize the title meta data. By default the scheduler will be initialized to 6 phases
    // with slots for 200 items. The title must be initialized in this manner prior to constructing
    // the MleIvStage; otherwise, the stage will crash.
    g_theTitle = new MleDirector();
    g_theTitle->m_theScheduler = new MleScheduler();
    g_theTitle->m_theScheduler->init();

    // Create an Inventor stage and initialize it.
    MleIvStage *stage = new MleIvStage();
    stage->init();

    // Main loop of execution.
    while(1)
    {
        g_theTitle->m_theScheduler->goAll();
    }

    // Clean up before exiting.
    delete stage;

    return 0;
}
