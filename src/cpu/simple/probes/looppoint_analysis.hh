/*
 * Copyright (c) 2023 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __CPU_SIMPLE_PROBES_LOOPPOINT_ANALYSIS_HH__
#define __CPU_SIMPLE_PROBES_LOOPPOINT_ANALYSIS_HH__

#include <map>
#include <queue>

#include "params/LooppointAnalysis.hh"
#include "params/LooppointAnalysisManager.hh"
#include "sim/sim_exit.hh"
// #include "sim/probe/probe.hh"
// #include "cpu/simple_thread.hh"
#include "arch/generic/pcstate.hh"
#include "cpu/probes/pc_count_pair.hh"
#include "debug/LooppointAnalysis.hh"
#include "cpu/base.hh"
#include "cpu/simple/probes/simpoint.hh"


namespace gem5
{

typedef std::pair<int, int> BBinfo;
typedef std::pair<Addr, Tick> NPCnTick;

class LooppointAnalysis : public ProbeListenerObject
{
  public:
    LooppointAnalysis(const LooppointAnalysisParams &params);

    virtual void regProbeListeners();

    void checkPc(const std::pair<SimpleThread*, StaticInstPtr>&);

  private:

    LooppointAnalysisManager *manager;

    Addr validAddrLowerBound;
    // The lower bound of the valid instruction address range

    Addr validAddrUpperBound;
    // The upper bound of the valid instruction address range

    int localInstCount;
    int BBInstCounter;

    BasicBlockRange currentBB;

    std::map<BasicBlockRange, BBinfo> localBBmap;

    std::queue<NPCnTick> mostRecentPc;

  public:

    std::vector<NPCnTick>
    getMostRecentPc() const
    {
      std::vector<NPCnTick> mostRecentPcVector;
      std::queue<NPCnTick> mostRecentPcCopy = mostRecentPc;
      while (!mostRecentPcCopy.empty()) {
        mostRecentPcVector.push_back(mostRecentPcCopy.front());
        mostRecentPcCopy.pop();
      }
      return mostRecentPcVector;
    }

    std::map<BasicBlockRange, BBinfo>
    getlocalBBmap() const
    {
      return localBBmap;
    }

};

class LooppointAnalysisManager : public SimObject 
{
  public:
    LooppointAnalysisManager(const LooppointAnalysisManagerParams &params);
    void countPc(Addr pc, int instCount);
    // void initListenerMap(int id, LooppointAnalysis* listenerAddr);

  private:

    std::map<Addr, int> counter;
    // The counter for all recorded PCs 

    std::queue<Addr> mostRecentPc;
    // Stores the 5 most recent incoming PCs

    int regionLength;

    Addr currentPc;
    // The most recent incoming PC

    // std::map<int, LooppointAnalysis*> listenerMap;

    int globalInstCount;

  public:
    std::map<Addr, int> 
    getCounter() const
    {
        return counter;
    }

    int
    getPcCount(Addr pc) const
    {
        if(counter.find(pc) != counter.end()) {
            return counter.find(pc)->second;
        }
        return -1;
    }

    Addr
    getCurrentPc() const
    {
      return currentPc;
    }

    // void test() const {
    //   int index = 0;
    //   while(listenerMap.find(index)!=listenerMap.end()) {
    //     LooppointAnalysis* ptr = listenerMap.find(index)->second;
    //     printf("inst in listener %i : %i\n", index, ptr->getLocalInstCounter());
    //     index ++;
    //   }
    // }

};




}

#endif // __CPU_SIMPLE_PROBES_LOOPPOINT_ANALYSIS_HH__
