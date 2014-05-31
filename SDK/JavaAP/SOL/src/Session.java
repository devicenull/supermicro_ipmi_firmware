import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.Date;
import javax.swing.JPanel;
import javax.swing.Timer;

public class Session extends JPanel implements Runnable {
	private VT100_PLUS vt;
	private User user;

	// 真實螢幕的尺寸
	private int width, height;

	private String iconname;
	private String windowtitle;

	// 連線資訊
	private String host;
	private int port;
	private int local_port;
	private int protocol;
	private Network network;

	private BufferedImage bi;
	private Resource resource;
	private Convertor conv;
	private S_O_L parent;

	// 防閒置用
	private boolean antiidle;
	private Timer ti;
	private long lastInputTime, antiIdleInterval;

	// 自動重連用
	private long startTime;

	// 這個 session 是否擁有一個 tab, 可能 session 還沒結束，但 tab 已被關閉。
	private boolean hasTab;

	/*
	 * 往上送到 parent 的
	 */

	public void showMessage(String msg) {
		// 當分頁仍存在時才會顯示訊息
		if (hasTab) {
			parent.showMessage(msg);
		}
	}

	public void showPopup(int x, int y) {
		Point p = getLocation();
		String link;

		if (coverURL(x, y)) {
			link = getURL(x, y);
		} else {
			link = null;
		}

		parent.showPopup(p.x + x, p.y + y, link);
	}

	public void openExternalBrowser(String url) {
		parent.openExternalBrowser(url);
	}

	public void changeSession(int index) {
		parent.changeSession(index);
	}

	public boolean isTabForeground() {
		return parent.isTabForeground(this);
	}

	public void setIcon(int icon) {
		parent.setIcon(icon, this);
	}

	public void setIconName(String in) {
		iconname = in;
		parent.updateTab();
	}

	public void setWindowTitle(String wt) {
		windowtitle = wt;
		parent.updateTab();

	}
	public void setLocalPort(int port) {
		local_port = port;

	}

	public void bell() {
		parent.bell();
	}

	public void copy() {
		parent.copy();
	}

	public void colorCopy() {
		parent.colorCopy();
	}

	public void paste() {
		parent.paste();
	}

	public void colorPaste() {
		parent.colorPaste();
	}

	public void writeByte(byte b) {
		lastInputTime = new Date().getTime();
		try {
			network.writeByte(b);
		} catch (IOException e) {

			{
				// 新建連線
				network = new Network(host, port, protocol);
				network.setLocalPort(local_port);
				if (network.connect() == false) {
					showMessage("連線失敗！");
					return;
				}

				// VT100_PLUS
				vt = new VT100_PLUS(width, height, network, this, bi, resource,
						conv);

				// User
				user = new User(this, resource);
				addKeyListener(user);
				addMouseListener(user);
				addMouseMotionListener(user);

				// 防閒置用的 Timer
				updateAntiIdleTime();
				lastInputTime = new Date().getTime();
				ti = new Timer(1000, new AntiIdleTask());
				ti.start();

				// 記錄連線開始的時間
				startTime = new Date().getTime();

				// 重連時也要設定 icon 為 connected icon
				setIcon(S_O_L.ICON_CONNECTED);

				// 連線成功後重繪螢幕
				repaint();

				// 取得 Focus
				requestFocusInWindow();

				// 一切準備妥當後開始處理來自網路的資料
				while (!network.isClosed()) {

					vt.parse();

					// buffer 裡的東西都處理完才重繪
					if (vt.isBufferEmpty()) {
						repaint();
					}
				}
			}
			System.out.println("Caught IOException in Session::writeByte(...)");
			close(true);
		}
	}

	public void writeBytes(byte[] buf, int offset, int len) {
		lastInputTime = new Date().getTime();
		try {
			network.writeBytes(buf, offset, len);
		} catch (IOException e) {
			System.out.println("Caught IOException in Session::writeBytes(...)");
			close(true);
		}
	}

	public void writeChar(char c) {
		byte[] buf = new byte[2];

		if (c > 127) {
			// FIXME: 寫死給 Big5
			conv.ucs2ToBig5Bytes(c, buf);
			writeBytes(buf, 0, buf.length);
		} else {
			writeByte((byte) c);
		}
	}

	public void writeChars(char[] buf, int offset, int len) {
		int count = 0;
		byte[] tmp = new byte[len * 2];
		byte[] tmp2 = new byte[2];

		for (int i = 0; i < len; i++) {
			// FIXME: 寫死給 Big5
			if (buf[i] > 127) {
				conv.ucs2ToBig5Bytes(buf[i], tmp2);
				tmp[count++] = tmp2[0];
				tmp[count++] = tmp2[1];
			} else {
				tmp[count++] = (byte) buf[i];
			}
		}

		writeBytes(tmp, 0, count);
	}

	public boolean isConnected() {
		// 如果 network 尚未建立則也當成尚未 connect.
		if (network == null) {
			return false;
		}
		return network.isConnected();
	}

	public boolean isClosed() {
		// 如果 network 尚未建立則也當成 closed.
		if (network == null) {
			return true;
		}
		return network.isClosed();
	}

	public boolean coverURL(int x, int y) {
		return vt.coverURL(x, y);
	}

	public String getURL(int x, int y) {
		return vt.getURL(x, y);
	}

	public void resetSelected() {
		vt.resetSelected();
	}

	public void selectConsequtive(int x, int y) {
		vt.selectConsequtive(x, y);
	}

	public void selectEntireLine(int x, int y) {
		vt.selectEntireLine(x, y);
	}

	public void setSelected(int x1, int y1, int x2, int y2) {
		vt.setSelected(x1, y1, x2, y2);
	}

	public String getSelectedText() {
		return vt.getSelectedText();
	}

	public String getSelectedColorText() {
		return vt.getSelectedColorText();
	}

	public void pasteText(String str) {
		vt.pasteText(str);
	}

	public void pasteColorText(String str) {
		vt.pasteColorText(str);
	}

	public int getKeypadMode() {
		return vt.getKeypadMode();
	}

	public void updateImage(BufferedImage b) {
		bi = b;
		if (vt != null) {
			vt.updateImage(b);
		}
	}

	public void updateSize() {
		Rectangle r = getVisibleRect();

		width = r.width;
		height = r.height;

		if (vt != null) {
			vt.updateSize(width, height);
		}
	}

	public void updateFont() {
		if (vt != null) {
			vt.updateFont();
		}
	}

	public void updateScreen() {
		if (vt != null) {
			vt.updateScreen();
		}
	}

	public String getHost() {
		return host;
	}

	public int getPort() {
		return port;
	}

	public int getProtocol() {
		return protocol;
	}

	public String getURL() {
		// String site = Network.proto[ protocol ] + "://" + host;
		String site = host;
		// 當 protocol 預設 port 不等於連線 port 時要顯示連線 port
		if ((protocol == Network.TELNET && port != 23)
				|| (protocol == Network.SSH && port != 22)) {
			site = site + ":" + port;
		}

		return site;
	}

	public String getIconName() {
		return iconname;
	}

	public String getWindowTitle() {
		return windowtitle;
	}

	public void updateAntiIdleTime() {
		// 更新是否需要啟動防閒置
		antiidle = resource.getBooleanValue(Resource.ANTI_IDLE);

		// 防閒置的作法是定時檢查距上次輸入是否超過 interval,
		// 所以這裡只要設定 antiIdleTime 就自動套用新的值了。
		antiIdleInterval = resource.getIntValue(Resource.ANTI_IDLE_INTERVAL) * 1000;
	}

	public void run() {
		// 新建連線
		network = new Network(host, port, protocol);
		network.setLocalPort(local_port);
		if (network.connect() == false) {
			showMessage("連線失敗！");
			return;
		}

		// VT100_PLUS
		vt = new VT100_PLUS(width, height, network, this, bi, resource, conv);
		//vt.setmargin(1, 25);

		// User
		user = new User(this, resource);
		addKeyListener(user);
		addMouseListener(user);
		addMouseMotionListener(user);

		// 防閒置用的 Timer
		updateAntiIdleTime();
		lastInputTime = new Date().getTime();
		ti = new Timer(1000, new AntiIdleTask());
		ti.start();

		// 記錄連線開始的時間
		startTime = new Date().getTime();

		// 重連時也要設定 icon 為 connected icon
		setIcon(S_O_L.ICON_CONNECTED);

		// 連線成功後重繪螢幕
		repaint();

		// 取得 Focus
		requestFocusInWindow();

		// 一切準備妥當後開始處理來自網路的資料
		while (!network.isClosed()) {

			vt.parse();

			// buffer 裡的東西都處理完才重繪
			if (vt.isBufferEmpty()) {
				repaint();
			}
		}
	}

	public synchronized void paintComponent(Graphics g) {
		super.paintComponent(g);

		// 因為很多分頁共用一張 image, 因此只有在前景的分頁才應該有繪圖的權利。
		// 這個 function 設成 synchronized 以確保切換到分頁後新的分頁後，
		// 原來前景的分頁的繪圖動作還沒完成，而干擾了畫面。

		// 雖然只要有從 server 讀資料進來就會呼叫 repaint(),
		// 但只有真正會被顯示出來的時候才真的叫 vt 畫出來。
		if (isTabForeground() && vt != null) {
			vt.paint(g);
		}
	}

	public void close(boolean fromRemote) {
//		javax.swing.JOptionPane.showMessageDialog(null, "Connection has been disconnected.");
		
		if (isClosed()) {
			return;
		}

		// 上層來的中斷連線命令，也就是來自使用者
		network.disconnect();

		// 斷線後應停止處理來自使用者的訊息
		removeKeyListener(user);
		removeMouseListener(user);
		removeMouseMotionListener(user);

		// 停止防閒置用的 timer
		ti.stop();

		// 通知 vt 停止運作
		vt.close();

		// 將連線 icon 改為斷線
		setIcon(S_O_L.ICON_CLOSED);

		// 若遠端 server 主動斷線則判斷是否需要重連
		boolean autoreconnect = resource
				.getBooleanValue(Resource.AUTO_RECONNECT);
		if (autoreconnect && fromRemote) {
			long reopenTime = resource
					.getIntValue(Resource.AUTO_RECONNECT_TIME);
			long reopenInterval = resource
					.getIntValue(Resource.AUTO_RECONNECT_INTERVAL);
			long now = new Date().getTime();

			// 判斷連線時間距現在時間是否超過自動重連時間
			// 若設定自動重連時間為 0 則總是自動重連
			if ((now - startTime <= reopenTime * 1000) || reopenTime == 0) {
				try {
					Thread.sleep(reopenInterval);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}

				parent.reopenSession(this);
			}
		}
		
		//System.exit(0);
	}

	public void remove() {
		// 設定分頁被移除了
		hasTab = false;
	}

	class AntiIdleTask implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			// 如果超過 antiIdelInterval milliseconds 沒有送出東西，
			// lastInputTime 在 writeByte, writeBytes 會被更新。
			long now = new Date().getTime();
			if (antiidle && isConnected()
					&& (now - lastInputTime > antiIdleInterval)) {
				// System.out.println( "Sent antiidle char" );
				// TODO: 設定 antiidle 送出的字元
				writeByte((byte) 0);
			}
		}
	}

	public Session(String h, int po, int pr, S_O_L pa, int wi, int he,
			BufferedImage b, Resource r, Convertor c) {
		super();

		host = h;
		port = po;
		protocol = pr;
		parent = pa;
		bi = b;
		resource = r;
		conv = c;

		// XXX: 預設成 host
		windowtitle = h;
		iconname = h;

		// 取消 focus traversal key, 這樣才能收到 tab.
		setFocusTraversalKeysEnabled(false);

		// 設定擁有一個分頁
		hasTab = true;

		setSize(wi, he);
		updateSize();
		// FIXME: magic number
		setBackground(Color.BLACK);
		setOpaque(true);
		setVisible(true);
		setFocusable(true);

	}

	@Override
	protected void processKeyEvent(KeyEvent ke) {
		// 解決F10彈出Menu的問題
		super.processKeyEvent(ke);
		ke.consume();
	}
}