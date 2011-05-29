package {
import cmodule.vecx.CLibInit;
public class Emulator{
	public var cloader:CLibInit;
	public var clib:Object;
	public function Emulator(){
		init();
	}
	private function init():void{
		cloader = new CLibInit();
		clib = cloader.init();
	}
}

}
