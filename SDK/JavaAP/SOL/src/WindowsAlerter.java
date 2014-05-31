public class WindowsAlerter {

	public native void alert(javax.swing.JFrame f);

	static {
		System.loadLibrary("WindowsAlerter");
	}
}
