#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"

extern int agentBodyType;

/*
 * Real game beaming.
 * Filling params x y angle
 */
void NaoBehavior::beam( double& beamX, double& beamY, double& beamAngle ) {
    beamX = -HALF_FIELD_X + worldModel->getUNum();
    beamY = 0;
    beamAngle = 0;
}


SkillType NaoBehavior::selectSkill() {
    // My position and angle
    //cout << worldModel->getUNum() << ": " << worldModel->getMyPosition() << ",\t" << worldModel->getMyAngDeg() << "\n";

    // Position of the ball
    //cout << worldModel->getBall() << "\n";

    // Example usage of the roboviz drawing system and RVSender in rvdraw.cc.
    // Agents draw the position of where they think the ball is
    // Also see example in naobahevior.cc for drawing agent position and
    // orientation.
    /*
    worldModel->getRVSender()->clear(); // erases drawings from previous cycle
    worldModel->getRVSender()->drawPoint("ball", ball.getX(), ball.getY(), 10.0f, RVSender::MAGENTA);
    */

    // ### Demo Behaviors ###

    // Walk in different directions
    //return goToTargetRelative(VecPosition(1,0,0), 0); // Forward
    //return goToTargetRelative(VecPosition(-1,0,0), 0); // Backward
    //return goToTargetRelative(VecPosition(0,1,0), 0); // Left
    //return goToTargetRelative(VecPosition(0,-1,0), 0); // Right
    //return goToTargetRelative(VecPosition(1,1,0), 0); // Diagonal
    //return goToTargetRelative(VecPosition(0,1,0), 90); // Turn counter-clockwise
    //return goToTargetRelative(VecPdosition(0,-1,0), -90); // Turn clockwise
    //return goToTargetRelative(VecPosition(1,0,0), 15); // Circle

    // Walk to the ball
    //return goToTarget(ball);

    // Turn in place to face ball
    /*double distance, angle;
    getTargetDistanceAndAngle(ball, distance, angle);
    if (abs(angle) > 10) {
      return goToTargetRelative(VecPosition(), angle);
    } else {
      return SKILL_STAND;
    }*/

    // Walk to ball while always facing forward
    //return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());

    // Dribble ball toward opponent's goal
    //return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));

    // Kick ball toward opponent's goal
    //return kickBall(KICK_FORWARD, VecPosition(HALF_FIELD_X, 0, 0)); // Basic kick
    //return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0)); // IK kick

    // Just stand in place
    //return SKILL_STAND;

    VecPosition posBall = ball;
    VecPosition target[6] ;
    
    target[0] = posBall + VecPosition(-0.5,0,0);    //控球位
    target[1] = VecPosition(-15,posBall.getY()*0.105,0);//守门位
    target[2] = posBall + VecPosition(9,4,0);   //接应位
    target[3] = posBall + VecPosition(9,-4,0);
    target[4] = (VecPosition(-15,0,0)+posBall)/2 + VecPosition(0, 0, 1).crossProduct((posBall-VecPosition(-15,0,0)).normalize())*5 ;    //防守位
    target[5] = (VecPosition(-15,0,0)+posBall)/2 - VecPosition(0, 0, 1).crossProduct((posBall-VecPosition(-15,0,0)).normalize())*5 ;    //利用叉积求中垂线上的点

    vector<vector<double>>dis(6, vector<double>(6,0));      
    int BotForTarget[6]={0};
    for(int i=0;i<6;i++){   //对无效位置进行调整
        if(target[i].getX()<-15){
            target[i].setX(-15);
        }else if(target[i].getX()>15){
            target[i].setX(15);
        }   //若x方向上超出边界
        
        if(target[i].getY()<-10){
            target[i].setY(-10);
        }else if(target[i].getY()>10){
            target[i].setY(10);
        }   //若y方向上超出边界
    }


    //绘图
    worldModel->getRVSender()->clearStaticDrawings();

    worldModel->getRVSender()->drawPoint("CM",target[0].getX()/*x坐标*/, target[0].getY()/*y坐标*/, RVSender::MAGENTA);
    worldModel->getRVSender()->drawPoint("GK",target[1].getX()/*x坐标*/, target[1].getY()/*y坐标*/, RVSender::MAGENTA);
    worldModel->getRVSender()->drawPoint("LW",target[2].getX()/*x坐标*/, target[2].getY()/*y坐标*/, RVSender::MAGENTA);
    worldModel->getRVSender()->drawPoint("RW",target[3].getX()/*x坐标*/, target[3].getY()/*y坐标*/, RVSender::MAGENTA);
    worldModel->getRVSender()->drawPoint("LCB",target[4].getX()/*x坐标*/, target[4].getY()/*y坐标*/, RVSender::MAGENTA);
    worldModel->getRVSender()->drawPoint("RCB",target[5].getX()/*x坐标*/, target[5].getY()/*y坐标*/, RVSender::MAGENTA);

    worldModel->getRVSender()->drawText("CM",target[0].getX()/*x坐标*/, target[0].getY()/*y坐标*/, RVSender::MAGENTA);
    worldModel->getRVSender()->drawText("GK",target[1].getX()/*x坐标*/, target[1].getY()/*y坐标*/, RVSender::MAGENTA);
    worldModel->getRVSender()->drawText("LW",target[2].getX()/*x坐标*/, target[2].getY()/*y坐标*/, RVSender::MAGENTA);
    worldModel->getRVSender()->drawText("RW",target[3].getX()/*x坐标*/, target[3].getY()/*y坐标*/, RVSender::MAGENTA);
    worldModel->getRVSender()->drawText("LCB",target[4].getX()/*x坐标*/, target[4].getY()/*y坐标*/, RVSender::MAGENTA);
    worldModel->getRVSender()->drawText("RCB",target[5].getX()/*x坐标*/, target[5].getY()/*y坐标*/, RVSender::MAGENTA);

    VecPosition pos;
    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            int playerNum = WO_TEAMMATE1 + j;
            WorldObject *tem = worldModel->getWorldObject(playerNum);
            if (worldModel->getUNum() == playerNum)
                pos = me;//若是遍历到"我",则获取"我"的位置
            else{
                tem = worldModel->getWorldObject(WO_TEAMMATE1 + j );
                pos = tem->pos;//若是队友则获取队友的位置
            }
            dis[i][j]=target[i].getDistanceTo(pos);//dis[i][j]保存第i个target到第j名球员的位置
            if(worldModel->getFallenTeammate(playerNum)){   //如果球员摔倒
                dis[i][j] ++;   //代价加1
            }
            if(tem->pos.getX()<-15||tem->pos.getX()>15||tem->pos.getY()<-10||tem->pos.getY()>10){   //不合法位置
                dis[i][j] = 10000;  //直接代价最大
            }
        }
    }

    for(int i=0;i<6;i++){
        int robot = min_element(dis[i].begin(),dis[i].begin()+6) - dis[i].begin();  //寻找最近机器人
        for(int j=0;j<6;j++){   
            dis[j][robot] = 10000;  //将最近机器人的代价设为最大，后续不再考虑
        }
        BotForTarget[i] = WO_TEAMMATE1 + robot; //将相应点位所需机器人编号存入数组
    }

    for(int i=0;i<6;i++){
        if(BotForTarget[i]==worldModel->getUNum()){ //若该点位所需球员是我
            if(i==0){   //若是控球位
                if(me.getDistanceTo(target[0])>1){  //如果我离控球位置较远
                    return goToTarget(target[0]);   //就先赶到控球位置
                }else{  //若离控球位置较近
                    if(target[2].getDistanceTo(posBall)>7||target[3].getDistanceTo(posBall)>7){ //若球不在接应点附近（此处，“附近”的定义为相距小于等于7）
                        if(me.getX()>0){    //如果不在本方半场内
                            if(me.getDistanceTo(target[2])<me.getDistanceTo(target[3])){
                                return kickBall(KICK_FORWARD,target[2]);   //踢到最近的接应点
                            }else{
                                return kickBall(KICK_FORWARD,target[3]);
                            }
                        }else{  //如果在本方半场内
                            double minDis = 10000;
                            int minDisTeammate = 0;
                            for(int j=0;j<6;j++){
                                int playerNum = WO_TEAMMATE1 + j;
                                WorldObject *tem = worldModel->getWorldObject(playerNum);
                                if(worldModel->getUNum()== playerNum) continue;
                                else{
                                    minDisTeammate = minDis < me.getDistanceTo(worldModel->getTeammate(WO_TEAMMATE1+j)) ? minDisTeammate : j;
                                }
                            }   //寻找最近的队友
                            return kickBall(KICK_FORWARD,worldModel->getTeammate(WO_TEAMMATE1+minDisTeammate));    //踢向最近的队友
                        }
                    }else{  //若球在接应点附近
                        VecPosition goalPost;   //寻找较近门柱
                        goalPost = me.getDistanceTo(VecPosition(15,1.05,0)) > me.getDistanceTo(VecPosition(15,-1.05,0)) ? VecPosition(15,1.05,0) : VecPosition(15,-1.05,0);
                        return kickBall(KICK_FORWARD,goalPost); //踢向最近门柱
                    }
                }
            }else{  //若我的点位不是控球位
                return goToTarget(target[i]);   //跑到相应位置
            }
        }
    }
}


/*
 * Demo behavior where players form a rotating circle and kick the ball
 * back and forth
 */
SkillType NaoBehavior::demoKickingCircle() {

    //Parameters for circle
    VecPosition center = VecPosition(-HALF_FIELD_X/2.0, 0, 0);
    double circleRadius = 5.0;
    double rotateRate = 2.5;

    //Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for(int i = WO_TEAMMATE1; i < WO_TEAMMATE1+NUM_AGENTS; ++i) {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
        } else {
            WorldObject* teammate = worldModel->getWorldObject( i );
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }

    if (playerClosestToBall == worldModel->getUNum()) {
        // Have closest player kick the ball toward the center
        return kickBall(KICK_FORWARD, center);
    } else {
        // Move to circle position around center and face the center
        VecPosition localCenter = worldModel->g2l(center);
        SIM::AngDeg localCenterAngle = atan2Deg(localCenter.getY(), localCenter.getX());

        // Our desired target position on the circle
        // Compute target based on uniform number, rotate rate, and time
        VecPosition target = center + VecPosition(circleRadius,0,0).rotateAboutZ(360.0/(NUM_AGENTS-1)*(worldModel->getUNum()-(worldModel->getUNum() > playerClosestToBall ? 1 : 0)) + worldModel->getTime()*rotateRate);

        // Adjust target to not be too close to teammates or the ball
        target = collisionAvoidance(true /*teammate*/, false/*opponent*/, true/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);

        if (me.getDistanceTo(target) < .25 && abs(localCenterAngle) <= 10) {
            // Close enough to desired position and orientation so just stand
            return SKILL_STAND;
        } else if (me.getDistanceTo(target) < .5) {
            // Close to desired position so start turning to face center
            return goToTargetRelative(worldModel->g2l(target), localCenterAngle);
        } else {
            // Move toward target location
            return goToTarget(target);
        }
    }
}


