#ifndef AMARA_SCRIPT
#define AMARA_SCRIPT

#include "amara.h"

namespace Amara {
    class Script: Amara::StateManager {
        public:
            Script(): Amara::StateManager() {

            }

            Script(bool willDelete): Script() {
                Script();
            }

            bool finished = false;
            
            virtual void finish() {
                finished = true;
            }
            void finishEvt() {
                if (evt()) {
                    finish();
                }
            }

            virtual void prepare(Amara::Entity& entity) {}
            virtual void script(Amara::Entity& entity) {}
    };
}

#endif