void setup()
{
}

enum robotState {
  waitToBegin,
  getRodLocations,
  moveToR1,
  getSpentRod1,
  moveSRod1ToSpent,
  getNewRod1,
  deliverRod1ToR1,
  moveToR2,
  getSpentRod2,
  moveSRod2ToSpent,
  getNewRod2,
  deliverRod2ToR2,
  celebrate
};

robotState robotMode;
boolean goOnFlag = false;
void loop()
{
  switch (robotMode)
  {
    //wait for blueTooth Communications to be established via pushbutton
    case waitToBegin:
    
    if(goOnFlag)
    {
      robotMode = getRodLocations;
      goOnFlag = false;
    }
    break;
    
    //determine rod locations and remember them
    case getRodLocations:
    
    if(goOnFlag)
    {
      robotMode = moveToR1;
      goOnFlag = false;
    }
    break;
    
    //go to the first reactor
    case moveToR1:
    
    if(goOnFlag)
    {
      robotMode = getSpentRod1;
      goOnFlag = false;
    }
    break;
    
    //get the rod from the reactor
    case getSpentRod1:
    
    if(goOnFlag)
    {
      robotMode = moveSRod1ToSpent;
      goOnFlag = false;
    }
    break;
    
    //bring the spent rod to an open storage tube
    case moveSRod1ToSpent:
    
    if(goOnFlag)
    {
      robotMode = getNewRod1;
      goOnFlag = false;
    }
    break;
    
    //move from spent rod depot to get a new rod
    case getNewRod1:
    
    if(goOnFlag)
    {
      robotMode = deliverRod1ToR1;
      goOnFlag = false;
    }
    break;
    
    //bring the fresh rod to reactor 1
    case deliverRod1ToR1:
    
    if(goOnFlag)
    {
      robotMode = moveToR2;
      goOnFlag = false;
    }
    break;
    
    //go to the second reactor
    case moveToR2:
    
    if(goOnFlag)
    {
      robotMode = getSpentRod2;
      goOnFlag = false;
    }
    break;
    
    //get the rod from the second reactor
    case getSpentRod2:
    
    if(goOnFlag)
    {
      robotMode = moveSRod2ToSpent;
      goOnFlag = false;
    }
    break;
    
    //bring the spent rod to an open storage tube
    case moveSRod2ToSpent:
    
    if(goOnFlag)
    {
      robotMode = getNewRod2;
      goOnFlag = false;
    }
    break;
    
    //move from spent rod depot to get a new rod
    case getNewRod2:
    
    if(goOnFlag)
    {
      robotMode = deliverRod2ToR2;
      goOnFlag = false;
    }
    break;
    
    //bring the fresh rod to reactor 2
    case deliverRod2ToR2:
    
    if(goOnFlag)
    {
      robotMode = celebrate;
      goOnFlag = false;
    }
    break;
    
    case celebrate:
    
    /*
                            ___
                        |  ~~--.
                        |%=@%%/
                        |o%%%/
                     __ |%%o/
               _,--~~ | |(_/ ._
            ,/'  m%%%%| |o/ /  `\.
           /' m%%o(_)%| |/ /o%%m `\
         /' %%@=%o%%%o|   /(_)o%%% `\
        /  %o%%%%%=@%%|  /%%o%%@=%%  \
       |  (_)%(_)%%o%%| /%%%=@(_)%%%  |
       | %%o%%%%o%%%(_|/%o%%o%%%%o%%% |
       | %%o%(_)%%%%%o%(_)%%%o%%o%o%% |
       |  (_)%%=@%(_)%o%o%%(_)%o(_)%  |
        \ ~%%o%%%%%o%o%=@%%o%%@%%o%~ /
         \. ~o%%(_)%%%o%(_)%%(_)o~ ,/
           \_ ~o%=@%(_)%o%%(_)%~ _/
             `\_~~o%%%o%%%%%~~_/'
    */
    while(1)
    {}
    break;
  }
}


    
