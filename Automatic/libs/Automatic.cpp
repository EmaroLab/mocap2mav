#include "Automatic.h"

#define PI              3.141592653589
#define Kland           2
#define THRE            0.15
#define DRATE_MIN       0.1
#define DRATE_MAX       0.4
#define VMAX            1
#define TMAX            2
#define TMIN            0.5
#define PLATFORM_OFFSET 0.1

Eigen::Quaterniond getQuatFromYaw(double yaw){

    Eigen::Quaterniond quaterniond;

    quaterniond.w() = cos(yaw/2);
    quaterniond.x() = 0;
    quaterniond.y() = 0;
    quaterniond.z() = sin(yaw/2);

    return quaterniond;

}

Automatic::Automatic()
{

}

MavState Automatic::getState()
{
    return _state;
}
exec::task Automatic::getTask()
{
    return _actualTask;
}

void Automatic::setState(MavState rState)
{
    _state = rState;

}

void Automatic::setTask(exec::task rTask)
{
    _actualTask.x=rTask.x;
    _actualTask.y=rTask.y;
    _actualTask.z=rTask.z;
    _actualTask.yaw=rTask.yaw;
    _actualTask.action=rTask.action;
    _actualTask.params[0]=rTask.params[0];
    _actualTask.params[1]=rTask.params[1];
    _actualTask.params[2]=rTask.params[2];
    _actualTask.params[3]=rTask.params[3];

}


void Automatic::rotate() {

    _comm.setType(MavState::type::POSITION);
    double angleValid = _actualTask.params[0];
    double yawSP = _actualTask.yaw;
    double yawComm;
    if(angleValid==0){

        double x_target = _actualTask.x;
        double y_target = _actualTask.y;

        yawSP = atan2(y_target - _state.getY(),x_target - _state.getX());

    }

    //calculateYawInterm(_state.getYawFromQuat(),yawSP,yawComm);

    yawComm = yawSP;


    _comm.setYaw(yawComm);

    Eigen::Quaterniond q_interm = getQuatFromYaw(yawComm);

    _comm.setOrientation(q_interm);

}

//QUA

void Automatic::calculateYawInterm(float heading, double yawTarget, double &yawComm){

    double yawSp_h = yawTarget - heading;



    _comm.setType(MavState::type::POSITION);
/*  DISABLE ALGORITHM FOR BUG FIXING
    if (fabs(yawSp_h) <= PI/10) yawComm = yawTarget;
    else if(fabs(yawSp_h) > PI - PI/18){
        //Increase yaw

        yawComm = heading + PI / 10 ;
        if (yawComm > PI){
            yawComm = yawComm - 2*PI;
        }
    }

    else{
        if (yawSp_h > 0){
            //Increase yaw

            yawComm = heading + PI / 10 ;
            if (yawComm > PI){
                yawComm = yawComm - 2*PI;
            }
        }
        else{
            //decrease yaw

            yawComm = heading - PI / 10 ;
            if (yawComm < -PI){
                yawComm = yawComm + 2*PI;
            }
        }
    }
    */
}

double calculateDescendRate(double dz,double drate_max,double drate_min, double tmax, double tmin){

    //Calculate the descend rate profile (linear) through y = mx + q
    //where x is vertical distance between robot and platform
    if(dz > tmax)      return drate_max;
    else if(dz < tmin) return drate_min;
    else{

        double m = (drate_max - drate_min) / (tmax - tmin);
        double q = drate_min - m * tmin;
        return m * dz + q;

    }

}

void Automatic::land2(MavState platPose, double landGain) {


    //Calculate difference

    double dx = - _state.getX() + platPose.getX();
    double dy = - _state.getY() + platPose.getY();
    double dz =   _state.getZ() + platPose.getZ() + PLATFORM_OFFSET; //_state.z is negative due to inversion,
                                                                    // that is why we dont use the minus sign

    // Be sure that we are on top of the target

    double x_target_v = Kland * (dx);
    double y_target_v = Kland * (dy);

    //Normalize

    Eigen::Vector2d v(x_target_v,y_target_v);

    if(v.norm() > VMAX){

        v.normalize();

        v = v * VMAX;

    }

    _comm.setVx(v(0));
    _comm.setVy(v(1));

    //TODO: add security checks on vz
    if(fabs(dx) <= THRE && fabs(dy) <= THRE){
        //Descending is safe, is it?<
        double desc = calculateDescendRate(-dz, DRATE_MAX, DRATE_MIN, TMAX, TMIN);
        desc = 0.25;
        double z_target_v = platPose.getVz() - desc;
        //if(z_target_v <= -DRATE) z_target_v = -DRATE; //could be useful

        double err_v = z_target_v - _state.getVz();

        z_target_v += landGain*(err_v); // Proportional term
        std::cout << "Actua: " <<-_state.getVz() + platPose.getVz()<< std::endl;
        std::cout << "Desir: " << desc << std::endl;
        std::cout << "ErrVe: " << err_v << std::endl;
        std::cout << "ErrDe: " << desc - (-_state.getVz() + platPose.getVz())<< std::endl;


        _comm.setVz(z_target_v);

    }else _comm.setVz(0); //Is it correct
    // ? Don't think so

    _comm.setType(MavState::type::VELOCITY);

}



void Automatic::takeOff() {

    _comm.setType(MavState::type::POSITION);
    double height = _actualTask.params[0];

    _comm.setX((float)_actualTask.x);
    _comm.setY((float)_actualTask.y);
    _comm.setZ((float)height);

    Eigen::Quaterniond q = getQuatFromYaw(_actualTask.yaw);

    _comm.setOrientation((float)q.w(),(float)q.x(),(float)q.y(),(float)q.z());

}

void Automatic::move()
{
    _comm.setType(MavState::type::POSITION);
    double alpha = _actualTask.params[0];

    calculatePositionInterm(alpha,_actualTask,_state,_comm);
}

void Automatic::calculatePositionInterm(const double alpha, const exec::task target, const MavState state, MavState &comm)
{

    _comm.setType(MavState::type::POSITION);
    double positionError[3] = {target.x - state.getX() ,target.y - state.getY() , target.z - state.getZ()};

    double dist = sqrt(pow(positionError[0],2) + pow(positionError[1],2) + pow(positionError[2],2));

    //Publish
    if(fabs(dist) <= alpha)
    {
        comm.setX((float)target.x);
        comm.setY((float)target.y);
        comm.setZ((float)target.z);
    }

    else if(fabs(dist) > alpha)
    {

        double incrementVect[3];
        //Normalize
        positionError[0] = positionError[0] / dist;
        positionError[1] = positionError[1] / dist;
        positionError[2] = positionError[2] / dist;

        //Calculate relative motion to actual position
        incrementVect[0] = positionError[0] * alpha;
        incrementVect[1] = positionError[1] * alpha;
        incrementVect[2] = positionError[2] * alpha;

        comm.setX(state.getX() + (float)incrementVect[0]);
        comm.setY(state.getY() + (float)incrementVect[1]);
        comm.setZ(state.getZ() + (float)incrementVect[2]);
    }
}

void Automatic::land1(float x_target, float y_target, float h) {

    //Calculate difference

    double dx = - _state.getX() + x_target;
    double dy = - _state.getY() + y_target;
    double dz =   _state.getZ() - h; //_state.z is negative due to inversion,
                                    // that is why we don't use the minus sign

    // Be sure that we are on top of the target

    double x_target_v = Kland * (dx);
    double y_target_v = Kland * (dy);

    //Normalize

    Eigen::Vector2d v(x_target_v,y_target_v);

    if(v.norm() > VMAX){

        v.normalize();

        v = v * VMAX;

    }

    _comm.setVx(v(0));
    _comm.setVy(v(1));

    //TODO: add security checks on vz
    if(fabs(dx) <= THRE && fabs(dy) <= THRE){
        //Descending is safe, is it?
        double desc = calculateDescendRate(fabs(dz), DRATE_MAX, DRATE_MIN, TMAX, TMIN);

        _comm.setVz(-desc);
        if (fabs(dz) < 0.05){
            _comm.setVx(0);
            _comm.setVy(0);
            _comm.setVz(-10);
        }

    }else _comm.setVz(0); //Is it correct? Don't think so

    _comm.setType(MavState::type::VELOCITY);

}
