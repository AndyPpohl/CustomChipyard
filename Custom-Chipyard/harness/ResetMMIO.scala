package chipyard.harness

import chisel3._
import freechips.rocketchip.diplomacy._
import freechips.rocketchip.tilelink._
import org.chipsalliance.cde.config.Parameters
import freechips.rocketchip.regmapper._
import freechips.rocketchip.amba.axi4._

class ResetMMIO(implicit p: Parameters) extends LazyModule {
    val device = new SimpleDevice("reset-mmio", Seq("ucb,reset-mmio"))

    val node = AXI4RegisterNode(
        address = AddressSet(0x60000000L, 0xFFF),
        //device = device,
        beatBytes = 8
    )

    lazy val module = new LazyModuleImp(this) {
        val io = IO(new Bundle {
            val resetReq = Output(Bool())
        })

    val reg = RegInit(false.B)
    io.resetReq := reg

    node.regmap(
        0x0 -> Seq(RegField.w(1,reg))
    )
    }
}