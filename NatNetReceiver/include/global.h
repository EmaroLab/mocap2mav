#ifndef GLOBAL_H
#define GLOBAL_H

#include "MavState.h"
#include <vector>
#include "utils.h"
//include "ManualControl.h"
//#include "PositionDispatcher.h"
//#include "Window.h"

//#include "CommanderThread.h"
#include "MavState.h"
#include "utils.h"

namespace g {
    extern MavState state;
    extern MavState setPoint;
    extern MavState platform;
}

extern std::vector <MavState> manualCommand;
extern std::vector <MavState> autoCommand;
extern std::vector<node> nodeList;
extern int actualNode;



namespace executioner{
    namespace land{
       extern bool land_sig;
       extern bool land_plat_sig;
       extern bool landed;
       extern bool was_executing;
       extern bool reset_int;


    }
    namespace take_off{
       extern bool take_off_sig;
       extern bool take_off_done;
       extern bool was_executing;

    }
    namespace move {
       extern bool move_sig;
       extern bool move_done;
       extern bool was_executing;
       extern bool keep_sp_still;
    }

    namespace rotate{
      extern bool rotate_sig;
      extern bool rotate_done;
      extern int rotate_id;
    }

    namespace trajectory{
      extern bool traj_sig;
      extern bool traj_done;
      extern bool was_executing;
      extern double traj_time;
      extern bool ready;
    }


}





#endif // GLOBAL_H

