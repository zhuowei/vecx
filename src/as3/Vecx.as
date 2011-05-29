package {
import flash.display.*;
import flash.utils.*;
import flash.events.*;
import cmodule.vecx.CLibInit;
public class Vecx extends Sprite{

	[Embed("../../rom.dat", mimeType="application/octet-stream")]
	public static const VectrexRom:Class;
	/** seconds between each tick for the emulator. Matchs the EMU_TIMER value defined in osint.c. */
	public static const EMU_TIMER:uint = 30;

	public var cloader:CLibInit;
	public var emu:Object;
	public var rom:ByteArray;
	public var timer:Timer;

	public function Vecx(){
		init();
	}
	private function init():void{
		cloader = new CLibInit();
		rom = (new VectrexRom() as ByteArray);
		cloader.supplyFile("rom.dat", rom);
		emu = cloader.init();
		var tickResult:int = emu.init();
		if(tickResult !=0){
			trace("Evil happened: init returned " + tickResult);
		}
		emu.tick();
		trace("first tick");
		timer = new Timer(EMU_TIMER);
		timer.addEventListener("timer", tickHandler);
		timer.start();
	}
	private function tickHandler(e:TimerEvent):void{
		emu.tick();
		updateGraphics();
	}
	private function updateGraphics():void{
	}
}

}
