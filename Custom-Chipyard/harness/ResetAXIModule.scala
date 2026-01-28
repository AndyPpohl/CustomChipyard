package chipyard.harness

import chisel3._
import chisel3.util._
import freechips.rocketchip.amba.axi4._
import org.chipsalliance.cde.config.Parameters
import freechips.rocketchip.tilelink.TLMessages

class ResetAXIModule extends Module {
  val io = IO(new Bundle {
      val a_valid = Input(Bool())
      val a_ready = Input(Bool())
      val a_addr = Input(UInt(64.W))
      val a_opcode = Input(UInt(3.W))
      //val w_valid = Input(Bool())
      //val w_ready = Input(Bool())
      val resetReq = Output(Bool())
  })

  val resetReg = RegInit(false.B)
  io.resetReq := resetReg

  //when (resetReg){
  //  resetReg := false.B
  //}

  val a_fire = io.a_valid && io.a_ready && (io.a_opcode === TLMessages.PutFullData)
  //val w_fire = io.w_valid && io.w_ready

  when(a_fire){
    printf("Im actually losing it\n")
  }

  when (a_fire && !resetReg) {
    printf("[RESET] AXI write addr=0x%x\n", io.a_addr)
    resetReg := true.B
  }
  //printf("[RESETMOD] a_v=%d a_r=%d w_v=%d w_r=%d addr=0x%x\n", io.a_valid, io.a_ready, io.w_valid, io.w_ready, io.a_addr)
}
