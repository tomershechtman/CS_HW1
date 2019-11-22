/* 046267 Computer Architecture - Winter 2019/20 - HW #1 */
/* This file should hold your implementation of the predictor simulator */

#include "bp_api.h"
#include <vector>

enum STATE {SNT, WNT, WT, ST};
enum HISTORY {GLOBAL_HISTORY, LOCAL_HISTORY};
enum TABLE {GLOBAL_TABLE, LOCAL_TABLE};
enum SHARE {NOT_USING_SHARE, USING_SHARE_MID, USING_SHARE_LSB};

class FSM {
private:
    STATE state;
public:
    FSM(int i) {
        switch(i) {
            case 0:
                state = SNT;
                break;
            case 1:
                state = WNT;
                break;
            case 2:
                state = WT;
                break;
            case 3:
                state = ST;
                break;
                //TODO: ADD EXCEPTION
        }
    }
    ~FSM() = default;
    void success () {
        switch(state) {
            case SNT:
                state = WNT;
                break;
            case WNT:
                state = WT;
                break;
            case WT:
                state = ST;
                break;
        }
    }
    void failed () {
        switch(state) {
            case ST:
                state = WT;
                break;
            case WT:
                state = WNT;
                break;
            case WNT:
                state = SNT;
                break;
        }
    }
};
class btbVector: public std::vector<unsigned> {
private:
    int tagSize, historySize;
    FSM fsm;
    std::vector<unsigned> tag;
    std::vector<unsigned> history;
public:
    btbVector(int tag_size, int history_size, int first_state): tagSize(tag_size),
                                                                historySize(history_size),
                                                                fsm(FSM(first_state)) {
        tag = std::vector<unsigned>(tagSize);
        history = std::vector<unsigned>(historySize, 0);
    }
    ~btbVector() = default;
    void success() {
        fsm.success();
    }
    void failed() {
        fsm.failed();
    }
    //TODO: ADD FEATURES TO SUCCESS AND FAILED
};
class BP {
private:
    int btbSize, tagSize, historySize, firstState;
    TABLE isGlobalTable;
    HISTORY isGlobalHistory;
    SHARE share;
    std::vector<btbVector> btb;
public:
    BP(int btb_size, int tag_size, int history_size, int first_state, bool isGT, bool isGH, int isShare) {
        btbSize = btb_size;
        tagSize = tag_size;
        historySize = history_size;
        firstState = first_state;
        switch (isGT) {
            case 1:
                isGlobalTable = GLOBAL_TABLE;
                break;
            case 0:
                isGlobalTable = LOCAL_TABLE;
                break;
        }
        switch (isGH) {
            case 1:
                isGlobalHistory = GLOBAL_HISTORY;
                break;
            case 0:
                isGlobalHistory = LOCAL_HISTORY;
                break;
        }
        switch (isShare) {
            case 0:
                share = NOT_USING_SHARE;
                break;
            case 1:
                share = USING_SHARE_LSB;
                break;
            case 2:
                share = USING_SHARE_MID;
                break;
        }
        btb = std::vector<btbVector>(btbSize, btbVector(tagSize, historySize, firstState));
    }
    ~BP();
    void success(int i) {
        btb[i].success();
    }
    void failed(int i) {
        btb[i].failed();
    }
};

BP* bp;

int BP_init(unsigned btbSize, unsigned historySize, unsigned tagSize, unsigned fsmState,
			bool isGlobalHist, bool isGlobalTable, int Shared){
    if(btbSize != 1 && btbSize != 2 && btbSize != 4 && btbSize != 8 && btbSize != 16 && btbSize != 32) {
        return -1;
    }
    if(historySize == 0 || historySize > 8) {
        return -1;
    }
    if(tagSize > 30) {
        return -1;
    }
    if(fsmState > 3) {
        return -1;
    }
    if(Shared > 2) {
        return -1;
    }
    bp = new BP(btbSize, tagSize, historySize, fsmState, isGlobalTable, isGlobalHist, Shared);
    if(!bp) {
        return -1;
    }
    return 0;
}

bool BP_predict(uint32_t pc, uint32_t *dst){

	return false;
}

void BP_update(uint32_t pc, uint32_t targetPc, bool taken, uint32_t pred_dst){
	return;
}

void BP_GetStats(SIM_stats *curStats){
	return;
}

