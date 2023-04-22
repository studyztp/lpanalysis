from gem5.simulate.exit_event import ExitEvent
from gem5.simulate.simulator import Simulator
from gem5.utils.requires import requires
from gem5.components.cachehierarchies.classic.no_cache import NoCache
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.memory.single_channel import SingleChannelDDR3_1600
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.isas import ISA
from gem5.resources.resource import obtain_resource, AbstractResource
from pathlib import Path
from m5.objects import LooppointAnalysis, LooppointAnalysisManager
import argparse

requires(isa_required=ISA.X86)

parser = argparse.ArgumentParser(
    description="simple test script for LP analysis"
)

parser.add_argument(
    "--binary",
    default="/mnt/c/gem5-related/benchmark/matrix-omp/matrix-omp",
    type=str,
    required=False,
    help="path to binary.",
)

args = parser.parse_args()

cache_hierarchy = NoCache()
memory = SingleChannelDDR3_1600(size="2GB")
processor = SimpleProcessor(
    cpu_type=CPUTypes.ATOMIC,
    isa=ISA.X86,
    num_cores=5,
)

lpmanager = LooppointAnalysisManager()

listenerid = 0

for core in processor.get_cores():
    lplistener = LooppointAnalysis()
    lplistener.ptmanager = lpmanager
    lplistener.listenerId = listenerid
    listenerid += 1
    core.core.probeListener = lplistener

board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy,
)
board.set_se_binary_workload(
    binary=AbstractResource(args.binary)
)

def printsth():
    mostRecentPc = lpmanager.getMostRecentPc()
    print("The 5 most recent PCs encountered and their count")
    for pc in mostRecentPc:
        count = lpmanager.getPcCount(pc)
        print(f"pc:{hex(pc)} count{count}\n")
    currentPc = lpmanager.getCurrentPc()
    print(f"The current PC is {currentPc}\n")
    print("try test\n")
    lpmanager.test()
    yield True


simulator = Simulator(
    board=board,
    on_exit_event={
        ExitEvent.WORKEND: printsth()
    }
)

simulator.run()
