// See LICENSE for license details.
package chipyard.fpga.arty100t

import org.chipsalliance.cde.config._
import freechips.rocketchip.subsystem._
import freechips.rocketchip.devices.debug._
import freechips.rocketchip.devices.tilelink._
import org.chipsalliance.diplomacy._
import org.chipsalliance.diplomacy.lazymodule._
import freechips.rocketchip.system._
import freechips.rocketchip.tile._
//import gemmini._

import org.chipsalliance.cde.config._
import freechips.rocketchip.subsystem._
import chipyard.config._
import gemmini._


import sifive.blocks.devices.uart._
import sifive.fpgashells.shell.{DesignKey}

import testchipip.serdes.{SerialTLKey}

import chipyard.{BuildSystem}

// don't use FPGAShell's DesignKey
class WithNoDesignKey extends Config((site, here, up) => {
  case DesignKey => (p: Parameters) => new SimpleLazyRawModule()(p)
})

/*
class WithoutPeripherals extends Config((site, here, up) => {
  case PeripheryUARTKey => Nil
  case PeripherySPIKey  => Nil
  case PeripheryGPIOKey => Nil
  case PeripheryI2CKey  => Nil
  case PeripheryPWMKey  => Nil
})
*/

// By default, this uses the on-board USB-UART for the TSI-over-UART link
// The PMODUART HarnessBinder maps the actual UART device to JD pin
class WithArty100TTweaks(freqMHz: Double = 50) extends Config(
  new WithArty100TPMODUART ++
  new WithArty100TUARTTSI ++
  new WithArty100TDDRTL ++
  new WithArty100TJTAG ++
  new WithNoDesignKey ++
  new testchipip.tsi.WithUARTTSIClient ++
  new chipyard.harness.WithSerialTLTiedOff ++
  new chipyard.harness.WithHarnessBinderClockFreqMHz(freqMHz) ++
  new chipyard.config.WithUniformBusFrequencies(freqMHz) ++
  new chipyard.harness.WithAllClocksFromHarnessClockInstantiator ++
  new chipyard.clocking.WithPassthroughClockGenerator ++
  new chipyard.config.WithTLBackingMemory ++ // FPGA-shells converts the AXI to TL for us
  new freechips.rocketchip.subsystem.WithExtMemSize(BigInt(256) << 20) ++ // 256mb on ARTY
  new freechips.rocketchip.subsystem.WithoutTLMonitors)

class RocketArty100TConfig extends Config(
  new WithArty100TTweaks ++
  new chipyard.config.WithBroadcastManager ++ // no l2
  new chipyard.RocketConfig)


class LeanGemminiArty100TConfig extends Config(
  new WithArty100TTweaks(freqMHz = 25) ++
  new freechips.rocketchip.rocket.WithoutFPU ++
  new freechips.rocketchip.rocket.WithNoBtb ++
  new freechips.rocketchip.rocket.WithoutMulDiv ++

  new freechips.rocketchip.rocket.WithL1ICacheSets(2) ++
  new freechips.rocketchip.rocket.WithL1DCacheSets(2) ++
  new freechips.rocketchip.rocket.WithL1ICacheWays(1) ++
  new freechips.rocketchip.rocket.WithL1DCacheWays(1) ++
  new freechips.rocketchip.subsystem.WithoutTLMonitors ++
  new freechips.rocketchip.subsystem.WithBufferlessBroadcastHub ++

  //new freechips.rocketchip.rocket.WithNSmallCores(1) ++

  new gemmini.DefaultGemminiConfig ++
  new chipyard.config.WithBroadcastManager ++ // no l2
  new chipyard.RocketConfig)

/*
class LeanGemminiArty100TConfig extends Config(
  // Board tweaks for Arty100T
  new WithArty100TTweaks ++

  // Small Gemmini accelerator
  new gemmini.LeanGemminiConfig ++

  // One small Rocket core (no BOOM)
  new chipyard.config.WithNBigCores(0) ++
  new chipyard.config.WithNSmallCores(1) ++

  // Remove L2 cache system
  new chipyard.config.WithNoL2 ++

  // Drop TL monitors to save logic
  new freechips.rocketchip.subsystem.WithoutTLMonitors ++

  // Basic broadcast manager for coherence
  new chipyard.config.WithBroadcastManager ++

  // Base Rocket config
  new chipyard.RocketConfig
)
*/
class NoCoresArty100TConfig extends Config(
  new WithArty100TTweaks ++
  new chipyard.config.WithBroadcastManager ++ // no l2
  new chipyard.NoCoresConfig)


// This will fail to close timing above 50 MHz
class BringupArty100TConfig extends Config(
  new WithArty100TSerialTLToGPIO ++
  new WithArty100TTweaks(freqMHz = 50) ++
  new testchipip.serdes.WithSerialTLPHYParams(testchipip.serdes.DecoupledInternalSyncSerialPhyParams(freqMHz=50)) ++
  new chipyard.ChipBringupHostConfig)
