package chipyard.config

import org.chipsalliance.cde.config.{Config}
import freechips.rocketchip.subsystem._
import freechips.rocketchip.tilelink._

class WithDefaultExtMemPort extends Config((site, here, up) => {
  case ExtMem => Some(MemoryPortParams(
    MasterPortParams(
      base      = 0x80000000L,                    // DRAM base
      size      = 0x10000000L,                    // 256 MiB
      beatBytes = site(MemoryBusKey).beatBytes,   // usually 8
      idBits    = 4
    ),
    1 // number of memory channels
  ))
})
