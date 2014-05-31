public class UtilUnicode {

	public static boolean isUnicode(byte data) {
		boolean isUnicode = false;
		
		if ((data & 0xf8) == 0xf0) {
			isUnicode = true;
		} else if ((data & 0xf0) == 0xe0) {
			isUnicode = true;
		} else if ((data & 0xe0) == 0xc0) {
			isUnicode = true;
		} else if ((data & 0x80) == 0x00) {
			isUnicode = false;
		}
		if(S_O_L.terminalType == 0)
			return isUnicode;
		else
			return false;
		
	}

	public static int bytesToEncode(byte data) {
		int bytes = 0;

		if ((data & 0xf8) == 0xf0) {
			bytes = 4;
		} else if ((data & 0xf0) == 0xe0) {
			bytes = 3;
		} else if ((data & 0xe0) == 0xc0) {
			bytes = 2;
		} else if ((data & 0x80) == 0x00) {
			bytes = 1;
		}

		return bytes;
	}
	
	public static boolean isHan(char inChar) {
		boolean isHan = false;
		
		if((0x3400 <= inChar && inChar <= 0x4DB5) ||
		   (0x4E00 <= inChar && inChar <= 0x9FA5) ||
		   (0x9FA6 <= inChar && inChar <= 0x9FBB) ||
		   (0xF900 <= inChar && inChar <= 0xFA2D) ||
		   (0xFA30 <= inChar && inChar <= 0xFA6A) ||
		   (0xFA70 <= inChar && inChar <= 0xFAD9) ||
		   (0x20000 <= inChar && inChar <= 0x2A6D6) ||
		   (0x2F800 <= inChar && inChar <= 0x2FA1D) ||
		   (0xFF00 <= inChar && inChar <= 0xFFEF) ||
		   (0x2E80 <= inChar && inChar <= 0x2EFF) ||
		   (0x3000 <= inChar && inChar <= 0x303F) ||
		   (0x31C0 <= inChar && inChar <= 0x31EF))
		{
			isHan = true;
		}
		
		return isHan;
	}
}
