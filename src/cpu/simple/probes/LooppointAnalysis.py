from m5.params import *
from m5.objects.Probe import ProbeListenerObject
from m5.objects import SimObject
from m5.util.pybind import *

class LooppointAnalysis(ProbeListenerObject):

    type = "LooppointAnalysis"
    cxx_header = "cpu/simple/probes/looppoint_analysis.hh"
    cxx_class = "gem5::LooppointAnalysis"

    cxx_exports = [
        PyBindMethod("getMostRecentPc"),
        PyBindMethod("getlocalBBmap"),
    ]

    ptmanager = Param.LooppointAnalysisManager("the PcCountAnalsi manager")
    # listenerId = Param.Int(0, "this is for manager to find the listener")

    validAddrRangeStart = Param.Addr(0, "the starting address of the valid "
                                     "insturction address range")
    validAddrRangeSize = Param.Addr(0, "the size of the valid address range")

class LooppointAnalysisManager(SimObject):

    type = "LooppointAnalysisManager"
    cxx_header = "cpu/simple/probes/looppoint_analysis.hh"
    cxx_class = "gem5::LooppointAnalysisManager"

    cxx_exports = [
        PyBindMethod("getCounter"),
        PyBindMethod("getPcCount"),
        # PyBindMethod("getMostRecentPc"),
        PyBindMethod("getCurrentPc"),
    ]

    regionLen = Param.Int(100000000, "each region's instruction length")

    
