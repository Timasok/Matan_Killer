    Node * dad = {};
    nodeCtor(&dad);
    Node * sonny = nodeConnect(dad, LEFT_SON);
    Node * daughter = nodeConnect(dad, RIGHT_SON);

    sonny->data = "pizzy hochy";
    daughter->data = "makarony zhelayu";
    dad->data = "poshli vy!";

    printIn(dad);