import java.awt.image.BufferedImage;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.net.*;
import java.util.*;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.util.Iterator;
import java.util.Vector;
import javax.swing.JPasswordField;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
//=============================Jim Chiu Start 20131028
import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

//=============================Jim Chiu end 20131028
public class S_O_L extends JFrame implements ActionListener, ChangeListener,
		KeyEventDispatcher, KeyListener, ComponentListener, ItemListener {

	public String SOL_VERSION =  "0.5.10";
	private int locationx, locationy;
	private int width, height;
	private int local_port;
	public static int terminalType = 0; // default terminal type: UTF-8
	// 0 --> UTF-8
	// 1 --> PCANSI
	public byte[] resPacket;

	private JTabbedPane tabbedPane;
	private JPanel controlPanel;

	private JMenuBar menuBar;
	private JMenu connectMenu, favoriteMenu, editMenu, optionMenu, helpMenu;
	private JMenuItem openItem, closeItem, reopenItem, quitItem;
	private JMenuItem copyItem, pasteItem, colorCopyItem, colorPasteItem;
	private JMenuItem preferenceItem, siteManagerItem, showToolbarItem;
	private JMenuItem usageItem, faqItem, aboutItem;
	private JMenuItem[] favoriteItems;

	private JPopupMenu popupMenu;
	private JMenuItem popupCopyItem, popupPasteItem, popupColorCopyItem,
			popupColorPasteItem, popupCopyLinkItem;

	private JToolBar connectionToolBar;
	private JScrollPane scrollPane;
	private JButton openButton, closeButton, reopenButton, resetButton,
			coldResetButton;
	private JButton copyButton, colorCopyButton, pasteButton, colorPasteButton;
	private JButton telnetButton, sshButton;
	private JLabel siteLabel;
	private JTextField siteField;

	private JLabel baudRateLabel;
	private JComboBox baudRateBox;
	private JComboBox terminalTypeBox;

	String[] rateList = new String[] { "9600", "19200", "38400", "57600",
			"115200" };
	String[] typeList = new String[] { "VT100", "PCANSI", "VT100+", "VT-UTF8" };

	private JLabel siteLabel_port;
	private JTextField siteField_port;
	// ID
	private JLabel siteLabel_ID;
	private JTextField siteField_ID;
	// PW
	private JLabel siteLabel_PW;
	private JPasswordField siteField_PW;
	// Privilege
	private JLabel siteLabel_Priv;
	private JTextField siteField_Priv;

	// radio button
	JRadioButton rb[] = new JRadioButton[2];

	private ImageIcon connectedIcon, closedIcon, bellIcon;

	private Resource resource;
	private Vector sessions;
	private Clip clip;
	private String colorText = null;

	// 是否顯示工具列
	private boolean showToolbar;

	// 按滑鼠右鍵時滑鼠下的連結
	private String tmpLink;

	private BufferedImage bi;
	private Convertor conv;

	private boolean isShowingDialog;

	private Network net;

	private keepAlive kAlive;
	private int kAliveRetry;

	private int solPort;
	private int userPriv;

	public static final int ICON_CONNECTED = 1;
	public static final int ICON_CLOSED = 2;
	public static final int ICON_ALERT = 3;

	static String IV = "marsmarsmarsmars";
	static String encryptionKey = "012s45m78c012m45";

	// input username or password
	public static String decrypt(String encryptedStr) {
		String decryptStr = "";
		try {
			decryptStr = aesdecrypt(toBinArray(encryptedStr), encryptionKey);

		} catch (Exception e) {
			e.printStackTrace();
		}
		System.out.println(encryptedStr + "," + decryptStr);

		return decryptStr;
	}

	public static String aesdecrypt(byte[] cipherText, String encryptionKey)
			throws Exception {
		Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding", "SunJCE");
		SecretKeySpec key = new SecretKeySpec(encryptionKey.getBytes("UTF-8"),
				"AES");
		cipher.init(Cipher.DECRYPT_MODE, key,
				new IvParameterSpec(IV.getBytes("UTF-8")));
		byte[] hasil = cipher.doFinal(cipherText);
		return (new String(hasil)).trim();
	}

	public static byte[] toBinArray(String hexStr) {

		char[] hexChars = hexStr.toCharArray();
		int startIndex = 0;
		int length = hexChars.length;

		byte[] result = new byte[length / 2];
		for (int j = 0; j < result.length; j++) {
			result[j] = (byte) (hexCharToNibble(hexChars[startIndex++]) * 16 + hexCharToNibble(hexChars[startIndex++]));
		}
		return result;
	}

	private static int hexCharToNibble(char ch) {
		if ((ch >= '0') && (ch <= '9'))
			return ch - '0';
		else if ((ch >= 'a') && (ch <= 'f'))
			return ch - 'a' + 10;
		else if ((ch >= 'A') && (ch <= 'F'))
			return ch - 'A' + 10;
		else
			throw new IllegalArgumentException("Not a hex char - '" + ch + "'");
	}

	// =============================Jim end
	// Danny++ 20120414
	private static byte[] HMACSHA1(byte[] data, int data_length, byte[] key,
			int key_length) {
		byte[] result = null;
		try {
			// Blocksize is 64 (bytes) when using one of the following hash
			// functions: SHA-1, MD5, RIPEMD-128/160
			byte[] newkey = new byte[64];
			byte[] o_key_pad = new byte[64];
			byte[] i_key_pad = new byte[64];

			// Using Standard SHA1 in HMAC
			MessageDigest md;
			md = MessageDigest.getInstance("SHA-1");

			// key = key ∥ [0x00 * (blocksize - length(key))]
			// default value is 0 --> Don't need pading
			if (key_length > 64) {
				// key = hash(key) should not enter here --> can be remove
				md.update(key, 0, key_length);
				byte[] temp = md.digest();
				md.reset();
				System.arraycopy(temp, 0, newkey, 0, temp.length);
			} else {
				System.arraycopy(key, 0, newkey, 0, key_length);
			}

			// o_key_pad = [0x5c * blocksize] ⊕ key
			// i_key_pad = [0x36 * blocksize] ⊕ key
			for (int i = 0; i < 64; i++) {
				o_key_pad[i] = (byte) (0x5c ^ newkey[i]);
				i_key_pad[i] = (byte) (0x36 ^ newkey[i]);
			}

			// hash(i_key_pad ∥ message)
			byte[] Hash1 = new byte[64 + data_length];
			System.arraycopy(i_key_pad, 0, Hash1, 0, 64);
			System.arraycopy(data, 0, Hash1, 64, data_length);
			md.update(Hash1, 0, Hash1.length);
			byte[] Hash1Result = md.digest();
			md.reset();

			// hash(o_key_pad ∥ hash(i_key_pad ∥ message))
			byte[] Hash2 = new byte[64 + Hash1Result.length];
			System.arraycopy(o_key_pad, 0, Hash2, 0, 64);
			System.arraycopy(Hash1Result, 0, Hash2, 64, Hash1Result.length);
			md.update(Hash2, 0, Hash2.length);
			result = md.digest();

		} catch (NoSuchAlgorithmException e) {
		}
		return result;
	}

	// End

	// 建立各種選單，包括 popup 選單也在這裡建立
	private void makeMenu() {
		menuBar = new JMenuBar();

		connectMenu = new JMenu("連線");
		connectMenu.setMnemonic(KeyEvent.VK_N);
		connectMenu.setToolTipText("meta-n");

		favoriteMenu = new JMenu("我的最愛");
		favoriteMenu.setMnemonic(KeyEvent.VK_F);
		favoriteMenu.setToolTipText("meta-f");

		editMenu = new JMenu("編輯");
		editMenu.setMnemonic(KeyEvent.VK_E);
		editMenu.setToolTipText("meta-e");

		optionMenu = new JMenu("選項");
		optionMenu.setMnemonic(KeyEvent.VK_O);
		optionMenu.setToolTipText("meta-o");

		helpMenu = new JMenu("求助");
		helpMenu.setMnemonic(KeyEvent.VK_H);
		helpMenu.setToolTipText("meta-h");

		popupMenu = new JPopupMenu();

		openItem = new JMenuItem("開新連線");
		openItem.setToolTipText("meta-q");
		openItem.addActionListener(this);

		closeItem = new JMenuItem("中斷連線");
		closeItem.setToolTipText("meta-w");
		closeItem.addActionListener(this);

		reopenItem = new JMenuItem("重新連線");
		reopenItem.setToolTipText("meta-r");
		reopenItem.addActionListener(this);

		quitItem = new JMenuItem("離開");
		quitItem.addActionListener(this);

		copyItem = new JMenuItem("複製");
		copyItem.addActionListener(this);
		copyItem.setToolTipText("meta-c");

		pasteItem = new JMenuItem("貼上");
		pasteItem.addActionListener(this);
		pasteItem.setToolTipText("meta-v");

		colorCopyItem = new JMenuItem("彩色複製");
		colorCopyItem.addActionListener(this);

		colorPasteItem = new JMenuItem("彩色貼上");
		colorPasteItem.addActionListener(this);

		preferenceItem = new JMenuItem("偏好設定");
		preferenceItem.addActionListener(this);
		preferenceItem.setToolTipText("meta-,");

		siteManagerItem = new JMenuItem("站台管理");
		siteManagerItem.addActionListener(this);
		siteManagerItem.setToolTipText("meta-.");

		showToolbarItem = new JMenuItem(showToolbar ? "隱藏工具列" : "顯示工具列");
		showToolbarItem.addActionListener(this);

		usageItem = new JMenuItem("使用說明");
		usageItem.addActionListener(this);

		faqItem = new JMenuItem("常見問題");
		faqItem.addActionListener(this);

		aboutItem = new JMenuItem("關於 S_O_L");
		aboutItem.addActionListener(this);

		setJMenuBar(menuBar);

		menuBar.add(connectMenu);
		menuBar.add(favoriteMenu);
		menuBar.add(editMenu);
		menuBar.add(optionMenu);
		menuBar.add(helpMenu);

		connectMenu.add(openItem);
		connectMenu.add(closeItem);
		connectMenu.add(reopenItem);
		connectMenu.add(quitItem);

		updateFavoriteMenu();

		editMenu.add(copyItem);
		editMenu.add(pasteItem);
		editMenu.add(colorCopyItem);
		editMenu.add(colorPasteItem);

		optionMenu.add(preferenceItem);
		optionMenu.add(siteManagerItem);
		optionMenu.add(showToolbarItem);

		helpMenu.add(usageItem);
		helpMenu.add(faqItem);
		helpMenu.add(aboutItem);
	}

	private void makeTabbedPane() {
		// tab 擺在上面，太多 tab 時,使用捲頁的顯示方式
		tabbedPane = new JTabbedPane(JTabbedPane.BOTTOM,
				JTabbedPane.SCROLL_TAB_LAYOUT);
		tabbedPane.addChangeListener(this);
		getContentPane().add(tabbedPane);
	}

	private void makeControlPanel() {
		controlPanel = new JPanel();
		controlPanel.setLayout(new FlowLayout(FlowLayout.CENTER));// TOOLBAR的位置

		connectionToolBar = new JToolBar();
		connectionToolBar.setVisible(showToolbar);
		connectionToolBar.setFloatable(false);
		connectionToolBar.setLayout(new FlowLayout());
		connectionToolBar.setRollover(true);

		closeButton = new JButton("Stop");
		closeButton.setToolTipText("meta-w");
		closeButton.setFocusable(false);
		closeButton.addActionListener(this);
		closeButton.setEnabled(false);

		reopenButton = new JButton("重連");
		reopenButton.setToolTipText("meta-r");
		reopenButton.setFocusable(false);
		reopenButton.addActionListener(this);

		copyButton = new JButton("複製");

		pasteButton = new JButton("貼上");

		colorCopyButton = new JButton("彩色複製");

		colorPasteButton = new JButton("彩色貼上");

		telnetButton = new JButton("Telnet");
		telnetButton.setToolTipText("meta-t");
		telnetButton.setFocusable(false);
		telnetButton.addActionListener(this);

		sshButton = new JButton("SSH");

		siteLabel = new JLabel("Host IP: ");
		siteLabel.setFocusable(false);
		siteLabel.setVisible(false);

		siteField = new JTextField(8);// TextField 長度
		siteField.addActionListener(this);
		siteField.addKeyListener(this);
		siteField.setVisible(false);

		siteLabel_port = new JLabel("Host Port: ");
		siteLabel_port.setFocusable(false);
		siteLabel_port.setVisible(false);

		siteField_port = new JTextField(8);// TextField 長度
		siteField_port.addActionListener(this);
		siteField_port.addKeyListener(this);
		siteField_port.setVisible(false);

		siteLabel_ID = new JLabel("ID: ");
		siteLabel_ID.setFocusable(false);
		siteLabel_ID.setVisible(false);

		siteField_ID = new JTextField(8);// TextField 長度
		siteField_ID.addActionListener(this);
		siteField_ID.addKeyListener(this);
		siteField_ID.setVisible(false);

		siteLabel_PW = new JLabel("PW: ");
		siteLabel_PW.setFocusable(false);
		siteLabel_PW.setVisible(false);

		siteField_PW = new JPasswordField(8);// TextField 長度
		siteField_PW.addActionListener(this);
		siteField_PW.addKeyListener(this);
		siteField_PW.setVisible(false);

		siteLabel_Priv = new JLabel("User Priv: ");
		siteLabel_Priv.setFocusable(false);
		siteLabel_Priv.setVisible(false);

		siteField_Priv = new JTextField(8);// TextField 長度
		siteField_Priv.addActionListener(this);
		siteField_Priv.addKeyListener(this);
		siteField_Priv.setVisible(false);

		openButton = new JButton("Start");
		openButton.setFocusable(false);
		openButton.addActionListener(this);

		resetButton = new JButton("Terminal Reset");
		resetButton.setFocusable(false);
		resetButton.addActionListener(this);
		resetButton.setEnabled(false);
		resetButton.setVisible(false);

		coldResetButton = new JButton("Hard Reset");
		coldResetButton.setFocusable(false);
		coldResetButton.addActionListener(this);
		coldResetButton.setEnabled(false);
		coldResetButton.setVisible(false);

		// baudRateLabel = new JLabel("Baud Rate ");
		baudRateLabel = new JLabel("Baud Rate (bps)");
		baudRateLabel.setFocusable(false);
		baudRateBox = new JComboBox(rateList);

		terminalTypeBox = new JComboBox(typeList);
		terminalTypeBox.setSelectedItem("VT100");
		terminalTypeBox.addItemListener(this);
		terminalTypeBox.setVisible(false);

		// add itemlistener after change display to default baud rate in
		// my_method()
		baudRateBox.setEnabled(false);

		// connectionToolBar.add(new JToolBar.Separator());

		// connectionToolBar.add(new JToolBar.Separator());

		// connectionToolBar.add(new JToolBar.Separator());
		//
		// connectionToolBar.add(siteLabel);
		// connectionToolBar.add(siteField);
		//
		// connectionToolBar.add(new JToolBar.Separator());
		// connectionToolBar.add(new JToolBar.Separator());
		// connectionToolBar.add(new JToolBar.Separator());
		// connectionToolBar.add(siteLabel_port);
		// connectionToolBar.add(siteField_port);
		// connectionToolBar.add(new JToolBar.Separator());
		// connectionToolBar.add(new JToolBar.Separator());
		// connectionToolBar.add(new JToolBar.Separator());
		//
		// connectionToolBar.add(siteLabel_ID);
		// connectionToolBar.add(siteField_ID);
		// connectionToolBar.add(new JToolBar.Separator());
		// connectionToolBar.add(new JToolBar.Separator());
		// connectionToolBar.add(new JToolBar.Separator());
		//
		// connectionToolBar.add(siteLabel_PW);
		// connectionToolBar.add(siteField_PW);
		// connectionToolBar.add(new JToolBar.Separator());

		// JRadioButton元件
		String str1[] = { "VT100_PLUS+", "VT-UTF8" };
		ButtonGroup bg1 = new ButtonGroup();

		for (int i = 0; i < 2; i++) {
			rb[i] = new JRadioButton(str1[i]);
			rb[i].setFont(new Font("華康中楷體", Font.BOLD, 12));
			rb[i].setForeground(Color.black);

			// connectionToolBar.add(rb[i]);
			bg1.add(rb[i]);

			// rb[i].setVisible(false);
		}
		rb[1].setSelected(true);// 預設VT-UTF8為初始
		// connectionToolBar.add(new JToolBar.Separator());
		connectionToolBar.add(baudRateLabel);
		// connectionToolBar.add(new JToolBar.Separator());
		connectionToolBar.add(baudRateBox);
		// connectionToolBar.add(new JToolBar.Separator());
		connectionToolBar.add(coldResetButton);
		// connectionToolBar.add(new JToolBar.Separator());
		connectionToolBar.add(resetButton);
		// connectionToolBar.add(new JToolBar.Separator());
		connectionToolBar.add(openButton);
		// connectionToolBar.add(new JToolBar.Separator());
		connectionToolBar.add(closeButton);

		connectionToolBar.add(new JToolBar.Separator());
		connectionToolBar.add(terminalTypeBox);

		// controlPanel.add(connectionToolBar);

		// scrollPane = new JScrollPane(
		// controlPanel,
		// JScrollPane.VERTICAL_SCROLLBAR_NEVER,
		// JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS
		// );
		// getContentPane().add(controlPanel, BorderLayout.SOUTH);
		// getContentPane().add(scrollPane, BorderLayout.SOUTH);
		getContentPane().add(connectionToolBar, BorderLayout.PAGE_START);
		// scrollPane.getHorizontalScrollBar().setVisible(false);
	}

	public void updateFavoriteMenu() {

	}

	public void updateToolbar() {

	}

	public void updateBounds() {
		locationx = resource.getIntValue(Resource.GEOMETRY_X);
		locationy = resource.getIntValue(Resource.GEOMETRY_Y);
		width = resource.getIntValue(Resource.GEOMETRY_WIDTH);
		height = resource.getIntValue(Resource.GEOMETRY_HEIGHT);

		setBounds(locationx, locationy, width, height);
		validate();
	}

	public void updateSize() {
		Session s;

		width = getWidth();
		height = getHeight();

		// 產生跟主視窗一樣大的 image
		bi = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);

		// 視窗大小調整時同步更新每個 session 的大小
		for (int i = 0; i < sessions.size(); i++) {
			s = (Session) sessions.elementAt(i);
			s.updateImage(bi);
			s.updateSize();
			s.updateFont();
			s.updateScreen();
		}
	}

	public void updateAntiIdleTime() {
		for (int i = 0; i < sessions.size(); i++) {
			((Session) sessions.elementAt(i)).updateAntiIdleTime();
		}
	}

	public void updateLookAndFeel() {
		try {
			// 使用系統的 Look and Feel
			if (resource.getBooleanValue(Resource.SYSTEM_LOOK_FEEL)) {
				UIManager.setLookAndFeel(UIManager
						.getSystemLookAndFeelClassName());
			} else {
				UIManager.setLookAndFeel(UIManager
						.getCrossPlatformLookAndFeelClassName());
			}
			SwingUtilities.updateComponentTreeUI(this);
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (UnsupportedLookAndFeelException e) {
			e.printStackTrace();
		}
	}

	public void updateTab() {
		// 為了下面 invokeLater 的關係，這邊改成 final
		final Session s = (Session) tabbedPane.getSelectedComponent();

		if (s != null) {

			// 修改視窗標題列
			setTitle("Java SOL Viewer ver." + SOL_VERSION);// +
															// s.getWindowTitle()
															// );

			// 修改網址列

			// 切換到已連線的 session 時設定 icon 為 connected, 以取消 bell icon.
			if (s.isClosed()) {
				setIcon(ICON_CLOSED, s);
			} else {
				setIcon(ICON_CONNECTED, s);
			}

			// 因為全部的連線共用一張 BufferedImage, 切換分頁時需重繪內容。
			s.updateScreen();

			// 讓所選的 session 取得 focus
			// XXX: s.requestFocusInWindow(); 無法正確取得 focus（似乎是 Java 的 bug）
			// 因此必須使用下面的 workaround
			javax.swing.SwingUtilities.invokeLater(new Runnable() {
				public void run() {
					s.requestFocusInWindow();
				}
			});

		} else {
			// FIXME: magic number
			setTitle("Java SOL Viewer v0.5.8");
			// siteField_ID.setText("");
			// siteField_PW.setText("");

		}
	}

	public void updateTabTitle() {
		for (int i = 0; i < tabbedPane.getTabCount(); i++) {
			tabbedPane
					.setTitleAt(
							i,
							(i + 1)
									+ "."
									+ ((Session) sessions.elementAt(i))
											.getHost());
		}
	}

	public void changeSession(int index) {
		if (0 <= index && index < tabbedPane.getTabCount()) {
			tabbedPane.setSelectedIndex(index);

		} else {

		}
	}

	public void showMessage(String msg) {

	}

	public void showPopup(int x, int y, String link) {

	}

	private void autoconnect() {
		Vector f = resource.getFavorites();
		Iterator iter = f.iterator();
		Site s;

		while (iter.hasNext()) {
			s = (Site) iter.next();
			if (s.autoconnect) {
				connect(s.host, s.port, s.protocol, -1);
			}
		}
	}

	private void connect(String h) {
		Site s;
		String host;
		int port, pos, prot;

		// 如果開新連線時按了取消則傳回值為 null
		if (h == null || h.length() == 0) {
			return;
		}

		do {
			// 透過 name or alias 連線
			s = resource.getFavorite(h);
			if (s != null) {
				host = s.host;
				port = s.port;
				prot = s.protocol;
				break;
			}

			pos = h.indexOf("://");
			// Default 就是 telnet
			prot = Network.TELNET;
			if (pos != -1) {
				if (h.substring(0, pos).equalsIgnoreCase("ssh")) {
					prot = Network.SSH;
				} else if (h.substring(0, pos).equalsIgnoreCase("telnet")) {
					prot = Network.TELNET;
				} else {
					showMessage("通訊協定錯誤！");
					return;
				}
				// 將 h 重設為 :// 後的東西
				h = h.substring(pos + 3);
			}

			// 取得 host:port, 或 host(:23)
			pos = h.indexOf(':');
			if (pos == -1) {
				host = h;

			} else {
				host = h.substring(0, pos);
				// port = Integer.parseInt( h.substring( pos + 1) );
			}
		} while (false);

		// host 長度為零則不做事
		if (h.length() == 0) {
			return;
		}
		port = Integer.parseInt(siteField_port.getText());
		connect(host, port, prot, -1);
	}

	private void connect(String host, int port, int protocol, int index) {
		Session s;

		// 大小由 tab 的可視範圍決定。
		// 但實際上每個 tab 可視範圍也沒這麼大，Session 內部會再自動調整（updateSize）
		Rectangle r = tabbedPane.getVisibleRect();
		s = new Session(host, port, protocol, this, r.width, r.height, bi,
				resource, conv);
		s.setLocalPort(local_port);
		// index 為連線後放在第幾個分頁，若為 -1 表開新分頁。
		if (index == -1) {
			sessions.add(s);
			/* 這行尹定要有否則分頁開不出來 */
			tabbedPane.addTab(host, closedIcon, s, host);
			tabbedPane.setSelectedIndex(tabbedPane.getTabCount() - 1);
		} else {
			sessions.setElementAt(s, index);
			tabbedPane.setComponentAt(index, s);
		}

		// 每個 session 都是一個 thread, 解決主程式被 block 住的問題。
		// 在 Session 的 run() 中會取得 Focus
		new Thread(s).start();
	}

	public void openExternalBrowser(String url) {
		int pos;

		String cmd = resource.getStringValue(Resource.EXTERNAL_BROWSER);
		if (cmd == null) {
			showMessage("瀏覽器啟動命令錯誤！");
			return;
		}

		// 把 %u 置換成給定的 url
		pos = cmd.indexOf("%u");
		if (pos == -1) {
			showMessage("瀏覽器啟動命令錯誤！");
			return;
		}
		cmd = cmd.substring(0, pos) + url + cmd.substring(pos + 2);

		runExternal(cmd);
	}

	public void runExternal(String cmd) {
		try {
			Runtime.getRuntime().exec(cmd);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void bell() {
		if (resource.getBooleanValue(Resource.USE_CUSTOM_BELL)) {
			try {
				java.applet.Applet.newAudioClip(
						new File(resource
								.getStringValue(Resource.CUSTOM_BELL_PATH))
								.toURL()).play();
			} catch (MalformedURLException e) {
				e.printStackTrace();
			}
		} else {
			java.awt.Toolkit.getDefaultToolkit().beep();
		}

		String os = System.getProperty("os.name");
		if (os.startsWith("Linux")) {
			requestFocus();
			updateTab();
		} else if (os.startsWith("Mac")) {
			// new MacAlerter().alert();
		} else if (os.startsWith("Windows")) {
			// new WindowsAlerter().alert(this);
		}
	}

	public void open() {
		String host = JOptionPane.showInputDialog(this, "請輸入連線位置：");
		connect(host);
	}

	public void quit() {
		for (int i = 0; i < sessions.size(); i++) {
			if (!((Session) sessions.elementAt(i)).isClosed()) {
				isShowingDialog = true;
				if (JOptionPane.showConfirmDialog(this, "還有連線尚未中斷，您確定要關閉程式嗎？",
						"連線中", JOptionPane.YES_NO_OPTION) != JOptionPane.YES_OPTION) {
					isShowingDialog = false;
					return;
				} else {
					isShowingDialog = false;
					break;
				}
			}
		}

		for (int i = 0; i < sessions.size(); i++) {
			((Session) sessions.elementAt(i)).close(false);
		}
		sessions.removeAllElements();

		// 紀錄結束時的視窗大小
		Rectangle r = getBounds();
		resource.setValue(Resource.GEOMETRY_X, r.x);
		resource.setValue(Resource.GEOMETRY_Y, r.y);
		resource.setValue(Resource.GEOMETRY_WIDTH, r.width);
		resource.setValue(Resource.GEOMETRY_HEIGHT, r.height);

		// 程式結束，把目前設定值寫回設定檔。
		resource.writeFile();

	}

	public void copy() {
		Session s = (Session) tabbedPane.getSelectedComponent();

		if (s != null) {
			String str = s.getSelectedText();
			if (str.length() != 0) {
				clip.setContent(str);
			}
			if (resource.getBooleanValue(Resource.CLEAR_AFTER_COPY)) {
				s.resetSelected();
				s.repaint();
			}
		}
	}

	public void colorCopy() {
		Session s = (Session) tabbedPane.getSelectedComponent();

		if (s != null) {
			String str = s.getSelectedColorText();
			if (str.length() != 0) {
				colorText = str;
			}
			if (resource.getBooleanValue(Resource.CLEAR_AFTER_COPY)) {
				s.resetSelected();
				s.repaint();
			}
		}
	}

	public void copyLink() {
		if (tmpLink != null) {
			clip.setContent(tmpLink);
		}
	}

	public void paste() {
		Session s = (Session) tabbedPane.getSelectedComponent();
		if (s != null) {
			s.pasteText(clip.getContent());
		}
	}

	public void colorPaste() {
		Session s = (Session) tabbedPane.getSelectedComponent();
		if (s != null && colorText != null) {
			s.pasteColorText(colorText);
		}
	}

	public void setIcon(int icon, Session s) {
		int index;
		ImageIcon ii;

		switch (icon) {
		case ICON_CONNECTED:
			ii = connectedIcon;
			break;
		case ICON_CLOSED:
			ii = closedIcon;
			break;
		case ICON_ALERT:
			ii = bellIcon;
			break;
		default:
			ii = null;
		}

		index = tabbedPane.indexOfComponent(s);
		if (index != -1) {
			tabbedPane.setIconAt(index, ii);
		}
	}

	public boolean isTabForeground(Session s) {
		return (tabbedPane.indexOfComponent(s) == tabbedPane.getSelectedIndex());
	}

	public boolean closeCurrentTab() {
		Session s = (Session) tabbedPane.getSelectedComponent();

		if (s != null) {

			// 連線中則詢問是否要斷線
			if (!s.isClosed()) {
				isShowingDialog = true;
				if (JOptionPane.showConfirmDialog(this, "Stop?", "Question",
						JOptionPane.YES_NO_OPTION) != JOptionPane.YES_OPTION) {
					isShowingDialog = false;
					return false;
				}
				isShowingDialog = false;

				// 通知 session 要中斷連線了
				s.close(false);
				kAlive.stop();

				if (!resource
						.getBooleanValue(Resource.REMOVE_MANUAL_DISCONNECT)) {
					return false;
				}
			}

			// 通知 session 要被移除了
			s.remove();

			tabbedPane.remove(s);
			sessions.remove(s);

			// 刪除分頁會影響分頁編號
			updateTabTitle();

			// 讓現在被選取的分頁取得 focus.
			updateTab();
		} else {
			// 沒有分頁了，關閉程式
			quit();
		}
		return true;
	}

	public void reopenSession(Session s) {
		if (s != null) {
			// 若連線中則開新分頁，已斷線則重連。
			if (s.isClosed()) {
				connect(s.getHost(), s.getPort(), s.getProtocol(),
						tabbedPane.indexOfComponent(s));
			} else {
				connect(s.getHost(), s.getPort(), s.getProtocol(), -1);
			}
		}
	}

	private void showPreference() {
		new Preference(this, resource);
	}

	private void showSiteManager() {
		new SiteManager(this, resource);
	}

	private void showUsage() {

	}

	private void showFAQ() {

	}

	private void showAbout() {

	}

	public String gethost() {
		String host = siteField.getText();

		return host;
	}

	public String getport() {
		String port = siteField_port.getText();

		return port;
	}

	public String getID() {
		String ID = siteField_ID.getText();
		return ID;
	}

	public String getPW() {
		String PW = siteField_PW.getText();
		return PW;
	}

	public byte ID_length() {
		int id_len = siteField_ID.getText().length();
		byte len = Byte.parseByte(String.valueOf(id_len));
		return len;
	}

	public int ID_length_int() {
		int id_len = siteField_ID.getText().length();
		return id_len;
	}

	public byte PW_length() {
		int pw_len = siteField_PW.getText().length();
		byte len = Byte.parseByte(String.valueOf(pw_len));
		return len;
	}

	public int PW_length_int() {
		int pw_len = siteField_PW.getText().length();
		return pw_len;
	}

	public static byte buf[] = new byte[26];

	public void my_method() {
		if (rb[1].isSelected() == true) {
			resetButton.setEnabled(true);
			coldResetButton.setEnabled(true);
		}
		if (rb[0].isSelected() == true) {
			resetButton.setEnabled(false);
			coldResetButton.setEnabled(true);
		}
		String host = siteField.getText();
		String port = siteField_port.getText();
		String ID = siteField_ID.getText();
		String PW = siteField_PW.getText();
		String Priv = siteField_Priv.getText();

		solPort = Integer.parseInt(port);
		userPriv = Integer.parseInt(Priv);
		// System.out.println("host:" + host);
		// System.out.println("port:" + port);
		// System.out.println("id  :" + ID);
		// System.out.println("pw  :" + PW);
		// System.out.println("===================");
		boolean b1 = !ID.equals("");
		boolean b2 = !PW.equals("");

		if (b1 && b2) {

			getchnlauthcap_client vtvt = new getchnlauthcap_client();
			vtvt.start();
			vtvt.interrupt();

			siteField.setEnabled(false);
			siteField_port.setEnabled(false);
			siteField_ID.setEnabled(false);
			siteField_PW.setEnabled(false);
			openButton.setEnabled(false);
			closeButton.setEnabled(true);

			while (vtvt.isAlive()) {
				;
			}
			// get current BMC baud rate
			byte[] payload = new byte[4];
			payload[0] = (byte) 0x01;
			payload[1] = (byte) 0x05;
			payload[2] = (byte) 0x00;
			payload[3] = (byte) 0x00;
			commandInfo cmdInfo = new commandInfo();
			cmdInfo.setCmdInfo(0x20, 0x30, 0x81, 0x00, 0x22, payload);
			sendCommand sc = new sendCommand();
			sc.setCommand(cmdInfo);
			sc.runInMainThread();

			int currentBaudRateIndex = 0;
			switch (sc.resPacket[24]) {
			case 0x06: {
				currentBaudRateIndex = 0;// 9600
				break;
			}
			case 0x07: {
				currentBaudRateIndex = 1;// 19200
				break;
			}
			case 0x08: {
				currentBaudRateIndex = 2;// 38400
				break;
			}
			case 0x09: {
				currentBaudRateIndex = 3;// 57600
				break;
			}
			case 0x0A: {
				currentBaudRateIndex = 4;// 115200
				break;
			}
			default: {
				currentBaudRateIndex = 1;// 19200;
			}
			}
			baudRateBox.setSelectedIndex(currentBaudRateIndex);
			baudRateBox.addItemListener(this);
			baudRateBox.setEnabled(true);

		} else {
			JOptionPane.showMessageDialog(this, "帳號或密碼輸入錯誤 !");// showMessage("通訊協定錯誤！");
			return;
		}

		// make a thread for keep alive heart beat
		kAliveRetry = 0;
		kAlive = new keepAlive();
		kAlive.start();
		// kAlive.interrupt();
	}

	public void actionPerformed(ActionEvent ae) {

		Object source = ae.getSource();

		if (source == resetButton) {
			int len_spkt = 6;

			buf[0] = (byte) 0x06; // Version: RMCP Version 1.0
			buf[1] = (byte) 0x00; // Reserved
			buf[2] = (byte) 0xff; // Sequence
			buf[3] = (byte) 0x07; // Class of Message
			buf[4] = (byte) 0x06;
			buf[5] = (byte) 0x01;

			buf[6] = (byte) inbuffer_osr[24];
			buf[7] = (byte) inbuffer_osr[25];
			buf[8] = (byte) inbuffer_osr[26];
			buf[9] = (byte) inbuffer_osr[27];
			buf[10] = ++ipmi_msg_1[10];
			buf[11] = (byte) 0x00;
			buf[12] = (byte) 0x00;
			buf[13] = (byte) 0x00;

			buf[14] = (byte) Byte.parseByte(String.valueOf(len_spkt + 4));
			buf[15] = (byte) 0x00;

			buf[16]++;
			buf[17] = (byte) 0x00;
			buf[18] = (byte) 0x00;
			buf[19] = (byte) 0x00;
			buf[20] = (byte) 0x1b;// ESC
			buf[21] = (byte) 0x52;// R
			buf[22] = (byte) 0x1b;// ESC
			buf[23] = (byte) 0x72;// r
			buf[24] = (byte) 0x1b;// ESC
			buf[25] = (byte) 0x52;// R

			DatagramPacket Packet;
			try {
				InetAddress address = InetAddress.getByName(gethost());
				Packet = new DatagramPacket(buf, buf.length, address, solPort);
				DatagramSocket socket = new DatagramSocket();
				socket.send(Packet);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		if (source == coldResetButton) {
			// construct command payload
			byte[] payload = new byte[1];
			payload[0] = (byte) 0x03;
			commandInfo cmdInfo = new commandInfo();
			cmdInfo.setCmdInfo(0x20, 0x00, 0x81, 0x00, 0x02, payload);
			sendCommand sc = new sendCommand();
			sc.setCommand(cmdInfo);
			sc.start();
			sc.interrupt();
		}

		if (source == openItem) {
			open();
		} else if (source == siteField || source == openButton) {
			my_method();
			openButton.setEnabled(false);
			closeButton.setEnabled(true);
			openButton.setFocusable(false);
		} else if (source == closeItem || source == closeButton) {
			if (closeCurrentTab()) {
				openButton.setEnabled(true);
				closeButton.setEnabled(false);
			}
			// 200902012, "Stop"之後把"Start"按鈕再Enable起來
		} else if (source == reopenItem || source == reopenButton) {
			reopenSession((Session) tabbedPane.getSelectedComponent());
		} else if (source == quitItem) {
			quit();
		} else if (source == copyItem || source == copyButton
				|| source == popupCopyItem) {
			copy();
		} else if (source == colorCopyItem || source == colorCopyButton
				|| source == popupColorCopyItem) {
			colorCopy();
		} else if (source == pasteItem || source == pasteButton
				|| source == popupPasteItem) {
			paste();
		} else if (source == colorPasteItem || source == colorPasteButton
				|| source == popupColorPasteItem) {
			colorPaste();
		} else if (source == popupCopyLinkItem) {
			copyLink();
		} else if (source == telnetButton) {
			siteField.setText("telnet://");
			siteField.requestFocusInWindow();
		} else if (source == sshButton) {
			siteField.setText("ssh://");
			siteField.requestFocusInWindow();
		} else if (source == preferenceItem) {
			showPreference();
		} else if (source == siteManagerItem) {
			showSiteManager();
		} else if (source == showToolbarItem) {
			showToolbar = !showToolbar;
			resource.setValue(Resource.SHOW_TOOLBAR, showToolbar);
			updateToolbar();
			updateSize();
		} else if (source == usageItem) {
			showUsage();
		} else if (source == faqItem) {
			showFAQ();
		} else if (source == aboutItem) {
			showAbout();
		}

	}

	public void stateChanged(ChangeEvent e) {
		// 切換分頁，更新視窗標題、畫面
		if (e.getSource() == tabbedPane) {
			updateTab();
		}
	}

	public boolean dispatchKeyEvent(KeyEvent ke) {

		// 只處理按下的狀況
		if (ke.getID() != KeyEvent.KEY_PRESSED) {
			return false;
		}

		// 如果正在顯示 dialog 則不要處理
		// 原本是期望 modal dialog 會 block 住 keyboard event 的，不過看來還是得自己判斷
		if (isShowingDialog) {
			return false;
		}

		if (ke.isAltDown() || ke.isMetaDown()) {

			if (ke.getKeyCode() == KeyEvent.VK_C) {
				copy();
			} else if (ke.getKeyCode() == KeyEvent.VK_D) {
				siteField.requestFocusInWindow();
				siteField.selectAll();
			} else if (ke.getKeyCode() == KeyEvent.VK_Q) {
				open();
			} else if (ke.getKeyCode() == KeyEvent.VK_R) {
				reopenSession((Session) tabbedPane.getSelectedComponent());
			} else if (ke.getKeyCode() == KeyEvent.VK_S) {
				siteField.setText("ssh://");
				siteField.requestFocusInWindow();
			} else if (ke.getKeyCode() == KeyEvent.VK_T) {

				siteField.requestFocusInWindow();
			} else if (ke.getKeyCode() == KeyEvent.VK_V) {
				paste();
			} else if (ke.getKeyCode() == KeyEvent.VK_W) {
				closeCurrentTab();
			} else if (KeyEvent.VK_0 <= ke.getKeyCode()
					&& ke.getKeyCode() <= KeyEvent.VK_9) {
				// 切換 tab 快速建
				// 可用 alt-N 或是 cmd-N 切換
				// XXX: 其實可以考慮 alt-0 不是快速建，不然多一個判斷很不舒服。

				if (KeyEvent.VK_0 == ke.getKeyCode()) {
					changeSession(9);
				} else {
					changeSession(ke.getKeyCode() - KeyEvent.VK_1);
				}
			} else if (ke.getKeyCode() == KeyEvent.VK_LEFT
					|| ke.getKeyCode() == KeyEvent.VK_Z) {
				// alt-left 切到上一個連線視窗
				// index 是否合法在 changeSession 內會判斷

				changeSession(tabbedPane.getSelectedIndex() - 1);
			} else if (ke.getKeyCode() == KeyEvent.VK_RIGHT
					|| ke.getKeyCode() == KeyEvent.VK_X) {
				// alt-right 切到下一個連線視窗
				// index 是否合法在 changeSession 內會判斷

				changeSession(tabbedPane.getSelectedIndex() + 1);
			} else if (ke.getKeyCode() == KeyEvent.VK_COMMA) {
				// alt-, 開啟偏好設定
				showPreference();
			} else if (ke.getKeyCode() == KeyEvent.VK_PERIOD) {
				// alt-. 開啟站台管理
				showSiteManager();
			} else {
				// 雖然按了 alt 或 meta, 但不認識，
				// 繼續往下送。
				return false;
			}

			// 功能鍵不再往下送
			return true;
		}

		// 一般鍵，繼續往下送。
		return false;
	}

	public void keyTyped(KeyEvent e) {
	}

	public void keyPressed(KeyEvent e) {
		if (e.getSource() == siteField) {
			if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
				updateTab();
			}
		}
	}

	public void keyReleased(KeyEvent e) {
	}

	public void itemStateChanged(ItemEvent e) {
		if (e.getSource() == baudRateBox) {
			int i;
			// Get input BaudRate
			Object o = e.getItem();
			// Send IPMI command to set baud rate of BMC
			if (e.getStateChange() == 1 && (String) e.getItem() != "Baud Rate") {
				i = Integer.parseInt((String) o);

				sendCommand sc = new sendCommand();
				// construct command payload
				byte[] payload = new byte[3];
				payload[0] = (byte) 0x01;// UART channel
				payload[1] = (byte) 0x05;// parameter selector

				switch (i) {
				case 9600: {
					payload[2] = (byte) 0x06;
					break;
				}
				case 19200: {
					payload[2] = (byte) 0x07;
					break;
				}
				case 38400: {
					payload[2] = (byte) 0x08;
					break;
				}
				case 57600: {
					payload[2] = (byte) 0x09;
					break;
				}
				case 115200: {
					payload[2] = (byte) 0x0A;
					break;
				}
				default: {
					payload[2] = (byte) 0x07;
				}
				}

				commandInfo cmdInfo = new commandInfo();
				cmdInfo.setCmdInfo(0x20, 0x30, 0x81, 0x00, 0x21, payload);
				sc.setCommand(cmdInfo);
				sc.start();
				sc.interrupt();
			}

		} else if (e.getSource() == terminalTypeBox) {
			if (e.getStateChange() == 1) {
				String cmd = (String) e.getItem();

				if (cmd.equals("VT100")) {
					terminalType = 0;
				} else if (cmd.equals("PCANSI")) {
					terminalType = 1;
				} else if (cmd.equals("VT100+")) {
					terminalType = 0;
				} else if (cmd.equals("VT-UTF8")) {
					terminalType = 0;
				}
			}
		}
	}

	public void componentResized(ComponentEvent ce) {
		updateSize();
	}

	public void componentMoved(ComponentEvent ce) {
	}

	public void componentShown(ComponentEvent ce) {
	}

	public void componentHidden(ComponentEvent ce) {
	}

	public S_O_L() {
		super("Java SOL Viewer v0.5.8");

		// 先用預設值初始化設定，若是在設定檔中有設定則會取代預設值。
		resource = new Resource();
		resource.initValue();
		resource.readFile();

		conv = new Convertor();

		// 各個連線
		sessions = new Vector();

		// 與系統剪貼簿溝通的橋樑
		clip = new Clip();

		// 設定 Look and Feel
		updateLookAndFeel();

		// 初始化各種 icon
		connectedIcon = new ImageIcon("icon/connected.png");
		closedIcon = new ImageIcon("icon/closed.png");
		bellIcon = new ImageIcon("icon/bell.png");

		// 是否要顯示工具列
		showToolbar = resource.getBooleanValue(Resource.SHOW_TOOLBAR);

		// 設定主畫面 Layout
		getContentPane().setLayout(new BorderLayout());

		makeTabbedPane();
		makeControlPanel();

		// 設定視窗位置、大小
		updateBounds();
		// 設定好視窗大小後才知道 image 大小
		bi = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);

		setVisible(true);
		setResizable(true);

		// 一開始沒有任何 dialog
		isShowingDialog = false;

		// 按下關閉視窗按鈕時只執行 quit()
		setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.exit(0);
			}

			public void windowActivated(WindowEvent e) {
				Session s;
				if (tabbedPane != null) {
					s = (Session) tabbedPane.getSelectedComponent();
					if (s != null)
						tabbedPane.addTab(s.getHost(), closedIcon, s,
								s.getHost());
				}
			}
		});

		// 接收視窗大小改變的事件
		addComponentListener(this);

		// 攔截鍵盤 event 以處理快速鍵
		DefaultKeyboardFocusManager.getCurrentKeyboardFocusManager()
				.addKeyEventDispatcher(this);

		// 自動連線
		autoconnect();

		// siteField.setText("192.168.59.177");
		// siteField_port.setText("solPort");
		// siteField_ID.setText("admin");
		// siteField_PW.setText("admin");

		siteField.setText("");
		siteField_port.setText("");
		siteField_ID.setText("");
		siteField_PW.setText("");
		siteField_Priv.setText("");
		int height = getHeight();
		pack();
		// setMaximumSize(new Dimension(600,600));
		// setMinimumSize(new Dimension(800,800));
		// setSize(getWidth(), height);
		setSize(980, 680);
	}

	public static void main(String args[]) {
		S_O_L main = new S_O_L();

		if (args.length == 5) {
			main.rb[0].setSelected(false);
			main.rb[1].setSelected(true);
			main.siteField.setText(args[0]);
			main.siteField_port.setText(args[1]);
			main.siteField_ID.setText(decrypt(args[2]));
			main.siteField_PW.setText(decrypt(args[3]));
			main.siteField_Priv.setText(args[4]);
			// main.connectionToolBar.setVisible(false);
			main.my_method();
			((Session) main.sessions.get(0)).setPreferredSize(new Dimension(
					960, 552));
			main.pack();
			// main.setSize(1168, 720);
			// main.setResizable(false);

		}
	}

	public static byte[] inbuffer_osr = new byte[256];
	public static byte[] ipmi_msg_1 = new byte[24];
	byte[] inbuffer_osr_1 = new byte[256];
	byte[] ipmi_msg_1_1 = new byte[24];
	byte[] inbuffer_osr_2 = new byte[256];
	byte[] ipmi_msg_1_2 = new byte[24];

	class commandInfo {
		int rsAddr, netFn_rsLUN, rqAddr, rqSeq_rqLUN, cmd;
		byte[] reqDatas;

		public void setCmdInfo(int _rsAddr, int _netFn_rsLUN, int _rqAddr,
				int _rqSeq_rqLUN, int _cmd, byte[] _reqDatas) {
			rsAddr = _rsAddr;
			netFn_rsLUN = _netFn_rsLUN;
			rqAddr = _rqAddr;
			rqSeq_rqLUN = _rqSeq_rqLUN;
			cmd = _cmd;
			reqDatas = new byte[_reqDatas.length];
			reqDatas = _reqDatas.clone();

		}
	}

	class sendCommand extends Thread {
		byte[] reqPacket;
		byte[] resPacket = new byte[256];

		public void setCommand(commandInfo cmdInfo) {
			byte checksum;
			int pLength;
			reqPacket = new byte[16 + 5 + 2 + cmdInfo.reqDatas.length];

			reqPacket[0] = (byte) 0x06;
			reqPacket[1] = (byte) 0x00;
			reqPacket[2] = (byte) 0xFF;
			reqPacket[3] = (byte) 0x07;

			reqPacket[4] = (byte) 0x06;// RMCP+
			reqPacket[5] = (byte) 0x00;// IPMI message
			reqPacket[6] = (byte) inbuffer_osr[24];// session number
			reqPacket[7] = (byte) inbuffer_osr[25];
			reqPacket[8] = (byte) inbuffer_osr[26];
			reqPacket[9] = (byte) inbuffer_osr[27];
			reqPacket[10] = (byte) ++ipmi_msg_1[10];// seq number
			reqPacket[11] = (byte) 0x00;
			reqPacket[12] = (byte) 0x00;
			reqPacket[13] = (byte) 0x00;

			reqPacket[14] = (byte) (reqPacket.length - 16);// payload length
			reqPacket[15] = (byte) 0x00;

			reqPacket[16] = (byte) cmdInfo.rsAddr;
			reqPacket[17] = (byte) cmdInfo.netFn_rsLUN;
			checksum = (byte) (reqPacket[16] + reqPacket[17]);
			checksum = (byte) ~checksum;
			reqPacket[18] = (byte) ++checksum;// first checksum
			reqPacket[19] = (byte) cmdInfo.rqAddr;
			reqPacket[20] = (byte) cmdInfo.rqSeq_rqLUN;
			reqPacket[21] = (byte) cmdInfo.cmd;
			pLength = reqPacket.length;
			checksum = (byte) (reqPacket[19] + reqPacket[20] + reqPacket[21]);
			for (int i = 22; i < pLength - 1; i++) {
				reqPacket[i] = cmdInfo.reqDatas[i - 22];
				checksum += reqPacket[i];
			}
			checksum = (byte) ~checksum;
			reqPacket[pLength - 1] = (byte) ++checksum;
		}

		public void run() {
			DatagramPacket reqP;
			try {
				DatagramSocket cmdSocket = new DatagramSocket();
				InetAddress address = InetAddress.getByName(gethost());
				reqP = new DatagramPacket(reqPacket, reqPacket.length, address,
						solPort);

				cmdSocket.send(reqP);
				// 接收
				reqP = new DatagramPacket(resPacket, resPacket.length);
				cmdSocket.setSoTimeout(1 * 1000);
				cmdSocket.receive(reqP);
				cmdSocket.close();
				// Reset the amount of retry when receiving something
				kAliveRetry = 0;

			} catch (IOException e) {
				kAliveRetry++;
				if (kAliveRetry >= 5) {
					Session s = (Session) tabbedPane.getSelectedComponent();
					s.close(true);
					kAlive.interrupt();
				}
			}
		}

		public void runInMainThread() {
			// execute IPMI command in main thread
			DatagramPacket reqP;
			try {
				DatagramSocket cmdSocket = new DatagramSocket();
				InetAddress address = InetAddress.getByName(gethost());
				reqP = new DatagramPacket(reqPacket, reqPacket.length, address,
						solPort);

				cmdSocket.send(reqP);
				// 接收
				reqP = new DatagramPacket(resPacket, resPacket.length);
				cmdSocket.setSoTimeout(1 * 1000);
				cmdSocket.receive(reqP);
				cmdSocket.close();
				// Reset the amount of retry when receiving something
				kAliveRetry = 0;

			} catch (IOException e) {
				kAliveRetry++;
				if (kAliveRetry >= 5) {
					Session s = (Session) tabbedPane.getSelectedComponent();
					s.close(true);
					kAlive.interrupt();
				}

			}
		}
	}

	class keepAlive extends Thread {
		public void run() {
			while (!Thread.currentThread().isInterrupted()) {
				try {
					sleep(3000);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				// Get Device ID
				byte[] payload = new byte[0];
				commandInfo cmdInfo = new commandInfo();
				cmdInfo.setCmdInfo(0x20, 0x18, 0x81, 0x00, 0x01, payload);
				sendCommand sc = new sendCommand();
				sc.setCommand(cmdInfo);
				sc.runInMainThread();
			}
		}
	};

	class getchnlauthcap_client extends Thread {

		public void run() {
			DatagramPacket Packet_gcac_1;
			Random randomG = new Random();

			int gcac_1 = 1;
			try {

				DatagramSocket socket_gcac_1 = new DatagramSocket();

				if (gcac_1 <= 1) {
					gcac_1++;

					byte[] rmcp_gcac_1 = new byte[23];
					// Get Cha. A.C.C.
					rmcp_gcac_1[0] = (byte) 0x06;
					rmcp_gcac_1[1] = (byte) 0x00;
					rmcp_gcac_1[2] = (byte) 0xFF;
					rmcp_gcac_1[3] = (byte) 0x07;

					rmcp_gcac_1[4] = (byte) 0x00;
					rmcp_gcac_1[5] = (byte) 0x00;
					rmcp_gcac_1[6] = (byte) 0x00;
					rmcp_gcac_1[7] = (byte) 0x00;
					rmcp_gcac_1[8] = (byte) 0x00;
					rmcp_gcac_1[9] = (byte) 0x00;
					rmcp_gcac_1[10] = (byte) 0x00;
					rmcp_gcac_1[11] = (byte) 0x00;
					rmcp_gcac_1[12] = (byte) 0x00;

					rmcp_gcac_1[13] = (byte) 0x09;
					rmcp_gcac_1[14] = (byte) 0x20;
					rmcp_gcac_1[15] = (byte) 0x18;

					rmcp_gcac_1[16] = (byte) 0xC8;
					rmcp_gcac_1[17] = (byte) 0x81;
					rmcp_gcac_1[18] = (byte) 0x00;
					rmcp_gcac_1[19] = (byte) 0x38;

					rmcp_gcac_1[20] = (byte) 0x8E;
					rmcp_gcac_1[21] = (byte) 0x04;

					byte zzz = ((byte) ((byte) rmcp_gcac_1[17]
							+ (byte) rmcp_gcac_1[18] + (byte) rmcp_gcac_1[19]
							+ (byte) rmcp_gcac_1[20] + (byte) rmcp_gcac_1[21]));
					byte zzzz = (byte) ~zzz;
					zzzz++;
					rmcp_gcac_1[22] = (byte) zzzz;

					InetAddress address = InetAddress.getByName(gethost());

					Packet_gcac_1 = new DatagramPacket(rmcp_gcac_1,
							rmcp_gcac_1.length, address, solPort);

					socket_gcac_1.send(Packet_gcac_1);

					// 接收
					byte[] inbuffer_gcac_1 = new byte[256];
					Packet_gcac_1 = new DatagramPacket(inbuffer_gcac_1,
							inbuffer_gcac_1.length);
					socket_gcac_1.receive(Packet_gcac_1);
					socket_gcac_1.close();

					if (inbuffer_gcac_1 != null) {
						DatagramPacket Packet_osr_1;

						int osr_1 = 1;
						try {

							DatagramSocket socket_osr_1 = new DatagramSocket();

							if (osr_1 <= 1) {
								osr_1++;

								byte[] rmcp_osr_1 = new byte[48];

								// Op Session Req
								rmcp_osr_1[0] = (byte) 0x06;
								rmcp_osr_1[1] = (byte) 0x00;
								rmcp_osr_1[2] = (byte) 0xFF;
								rmcp_osr_1[3] = (byte) 0x07;

								rmcp_osr_1[4] = (byte) 0x06;
								rmcp_osr_1[5] = (byte) 0x10;

								rmcp_osr_1[6] = (byte) 0x00;
								rmcp_osr_1[7] = (byte) 0x00;
								rmcp_osr_1[8] = (byte) 0x00;
								rmcp_osr_1[9] = (byte) 0x00;

								rmcp_osr_1[10] = (byte) 0x00;
								rmcp_osr_1[11] = (byte) 0x00;
								rmcp_osr_1[12] = (byte) 0x00;
								rmcp_osr_1[13] = (byte) 0x00;

								rmcp_osr_1[14] = (byte) 0x20;
								rmcp_osr_1[15] = (byte) 0x00;

								rmcp_osr_1[16] = (byte) 0x07;
								rmcp_osr_1[17] = (byte) 0x04;
								rmcp_osr_1[18] = (byte) 0x00;
								rmcp_osr_1[19] = (byte) 0x00;

								rmcp_osr_1[20] = (byte) 0xa4;
								rmcp_osr_1[21] = (byte) 0xa3;

								rmcp_osr_1[22] = (byte) 0xa2;
								rmcp_osr_1[23] = (byte) 0xa1;

								rmcp_osr_1[24] = (byte) 0x00;
								rmcp_osr_1[25] = (byte) 0x00;
								rmcp_osr_1[26] = (byte) 0x00;
								rmcp_osr_1[27] = (byte) 0x08;
								rmcp_osr_1[28] = (byte) 0x01;
								rmcp_osr_1[29] = (byte) 0x00;
								rmcp_osr_1[30] = (byte) 0x00;
								rmcp_osr_1[31] = (byte) 0x00;

								rmcp_osr_1[32] = (byte) 0x01;
								rmcp_osr_1[33] = (byte) 0x00;
								rmcp_osr_1[34] = (byte) 0x00;
								rmcp_osr_1[35] = (byte) 0x08;
								rmcp_osr_1[36] = (byte) 0x00;
								rmcp_osr_1[37] = (byte) 0x00;
								rmcp_osr_1[38] = (byte) 0x00;
								rmcp_osr_1[39] = (byte) 0x00;

								rmcp_osr_1[40] = (byte) 0x02;
								rmcp_osr_1[41] = (byte) 0x00;
								rmcp_osr_1[42] = (byte) 0x00;
								rmcp_osr_1[43] = (byte) 0x08;
								rmcp_osr_1[44] = (byte) 0x00;
								rmcp_osr_1[45] = (byte) 0x00;
								rmcp_osr_1[46] = (byte) 0x00;
								rmcp_osr_1[47] = (byte) 0x00;

								Packet_osr_1 = new DatagramPacket(rmcp_osr_1,
										rmcp_osr_1.length, address, solPort);

								socket_osr_1.send(Packet_osr_1);

								// 接收
								Packet_osr_1 = new DatagramPacket(
										inbuffer_osr_1, inbuffer_osr_1.length);
								socket_osr_1.receive(Packet_osr_1);
								socket_osr_1.close();

								if (inbuffer_osr_1 != null) {

									DatagramPacket Packet_rakp1_1;

									int rakp1_1 = 1;
									try {

										DatagramSocket socket_rakp1_1 = new DatagramSocket();

										if (rakp1_1 <= 1) {
											rakp1_1++;

											byte[] rmcp_rakp1_1 = new byte[60];

											// RAKP Msg 1
											rmcp_rakp1_1[0] = (byte) 0x06; // IMPI
																			// header
											rmcp_rakp1_1[1] = (byte) 0x00;
											rmcp_rakp1_1[2] = (byte) 0xFF;
											rmcp_rakp1_1[3] = (byte) 0x07;
											rmcp_rakp1_1[4] = (byte) 0x06; // AT/F
																			// 6h
																			// format
																			// =
																			// RMCP+
											rmcp_rakp1_1[5] = (byte) 0x12; // Payload
																			// Type
																			// 1101
																			// 0000
											rmcp_rakp1_1[6] = (byte) 0x00; // IMPI
																			// RMCP+
																			// Session
																			// ID
											rmcp_rakp1_1[7] = (byte) 0x00;
											rmcp_rakp1_1[8] = (byte) 0x00;
											rmcp_rakp1_1[9] = (byte) 0x00;
											rmcp_rakp1_1[10] = (byte) 0x00; // Session
																			// Seq.
																			// No.
											rmcp_rakp1_1[11] = (byte) 0x00;
											rmcp_rakp1_1[12] = (byte) 0x00;
											rmcp_rakp1_1[13] = (byte) 0x00;
											rmcp_rakp1_1[14] = (byte) 0x2C; // IMPI
																			// payload
																			// length
											rmcp_rakp1_1[15] = (byte) 0x00;
											// Msg Tag
											rmcp_rakp1_1[16] = (byte) 0x08;
											rmcp_rakp1_1[17] = (byte) 0x00; // byte
																			// 2:4
																			// -
																			// reserved
											rmcp_rakp1_1[18] = (byte) 0x00;
											rmcp_rakp1_1[19] = (byte) 0x00;

											rmcp_rakp1_1[20] = (byte) inbuffer_osr_1[24];
											rmcp_rakp1_1[21] = (byte) inbuffer_osr_1[25];
											rmcp_rakp1_1[22] = (byte) inbuffer_osr_1[26];
											rmcp_rakp1_1[23] = (byte) inbuffer_osr_1[27];

											// Remote Console Random Number,自己產生
											rmcp_rakp1_1[24] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[25] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[26] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[27] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[28] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[29] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[30] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[31] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[32] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[33] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[34] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[35] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[36] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[37] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[38] = (byte) randomG
													.nextInt(128);
											rmcp_rakp1_1[39] = (byte) randomG
													.nextInt(128);

											rmcp_rakp1_1[40] = (byte) userPriv; // Level

											rmcp_rakp1_1[41] = (byte) 0x00; // byte
																			// 2:4
																			// -
																			// reserved
											rmcp_rakp1_1[42] = (byte) 0x00;

											rmcp_rakp1_1[43] = (byte) ID_length();// User
																					// Name
																					// Length

											byte id_byte[] = siteField_ID
													.getText().getBytes();// User
																			// Name,ASCII
																			// character

											rmcp_rakp1_1[44] = (byte) 0x00;
											rmcp_rakp1_1[45] = (byte) 0x00;
											rmcp_rakp1_1[46] = (byte) 0x00;
											rmcp_rakp1_1[47] = (byte) 0x00;
											rmcp_rakp1_1[48] = (byte) 0x00;
											rmcp_rakp1_1[49] = (byte) 0x00;
											rmcp_rakp1_1[50] = (byte) 0x00;
											rmcp_rakp1_1[51] = (byte) 0x00;
											rmcp_rakp1_1[52] = (byte) 0x00;
											rmcp_rakp1_1[53] = (byte) 0x00;
											rmcp_rakp1_1[54] = (byte) 0x00;
											rmcp_rakp1_1[55] = (byte) 0x00;
											rmcp_rakp1_1[56] = (byte) 0x00;
											rmcp_rakp1_1[57] = (byte) 0x00;
											rmcp_rakp1_1[58] = (byte) 0x00;
											rmcp_rakp1_1[59] = (byte) 0x00;

											int id = 0;
											for (id = 0; id < ID_length_int(); id++) {
												rmcp_rakp1_1[44 + id] = (byte) id_byte[id];

											}

											Packet_rakp1_1 = new DatagramPacket(
													rmcp_rakp1_1,
													rmcp_rakp1_1.length,
													address, solPort);

											socket_rakp1_1.send(Packet_rakp1_1);

											// 接收
											byte[] inbuffer_rakp1_1 = new byte[256];
											Packet_rakp1_1 = new DatagramPacket(
													inbuffer_rakp1_1,
													inbuffer_rakp1_1.length);
											socket_rakp1_1
													.receive(Packet_rakp1_1);

											if (inbuffer_rakp1_1 != null)

											{

												DatagramPacket Packet_rakp3_1;

												int rakp3_1 = 1;
												try {

													DatagramSocket socket_rakp3_1 = new DatagramSocket();

													if (rakp3_1 <= 1) {
														rakp3_1++;

														// Data and Keys to Auth
														// code
														byte[] Data = new byte[40];
														byte[] Key = new byte[25];
														Data[0] = (byte) inbuffer_rakp1_1[24];
														Data[1] = (byte) inbuffer_rakp1_1[25];
														Data[2] = (byte) inbuffer_rakp1_1[26];
														Data[3] = (byte) inbuffer_rakp1_1[27];
														Data[4] = (byte) inbuffer_rakp1_1[28];
														Data[5] = (byte) inbuffer_rakp1_1[29];
														Data[6] = (byte) inbuffer_rakp1_1[30];
														Data[7] = (byte) inbuffer_rakp1_1[31];
														Data[8] = (byte) inbuffer_rakp1_1[32];
														Data[9] = (byte) inbuffer_rakp1_1[33];
														Data[10] = (byte) inbuffer_rakp1_1[34];
														Data[11] = (byte) inbuffer_rakp1_1[35];
														Data[12] = (byte) inbuffer_rakp1_1[36];
														Data[13] = (byte) inbuffer_rakp1_1[37];
														Data[14] = (byte) inbuffer_rakp1_1[38];
														Data[15] = (byte) inbuffer_rakp1_1[39];
														Data[16] = (byte) inbuffer_rakp1_1[20];
														Data[17] = (byte) inbuffer_rakp1_1[21];
														Data[18] = (byte) inbuffer_rakp1_1[22];
														Data[19] = (byte) inbuffer_rakp1_1[23];
														Data[20] = (byte) userPriv;
														Data[21] = (byte) ID_length();

														byte pw_byte[] = siteField_PW
																.getText()
																.getBytes();
														byte id1_byte[] = siteField_ID
																.getText()
																.getBytes();

														Data[22] = (byte) 0x00;
														Data[23] = (byte) 0x00;
														Data[24] = (byte) 0x00;
														Data[25] = (byte) 0x00;
														Data[26] = (byte) 0x00;
														Data[27] = (byte) 0x00;
														Data[28] = (byte) 0x00;
														Data[29] = (byte) 0x00;
														Data[30] = (byte) 0x00;
														Data[31] = (byte) 0x00;
														Data[32] = (byte) 0x00;
														Data[33] = (byte) 0x00;
														Data[34] = (byte) 0x00;
														Data[35] = (byte) 0x00;
														Data[36] = (byte) 0x00;
														Data[37] = (byte) 0x00;
														Data[38] = (byte) 0x00;
														Data[39] = (byte) 0x00;

														int pw = 0;
														for (pw = 0; pw < PW_length_int(); pw++) {
															Key[pw] = (byte) pw_byte[pw];
														}

														for (pw = 0; pw < ID_length_int(); pw++) {
															Data[22 + pw] = (byte) id1_byte[pw];
														}

														int Key_Length = PW_length_int();
														int ID_Length = ID_length_int();
														int Data_Length = 22 + ID_Length;

														byte[] Authcode = HMACSHA1(
																Data,
																Data_Length,
																Key, Key_Length);

														byte[] rmcp_rakp3_1 = new byte[44];

														// RAKP Msg 3
														rmcp_rakp3_1[0] = (byte) 0x06; // IMPI
																						// header
														rmcp_rakp3_1[1] = (byte) 0x00;
														rmcp_rakp3_1[2] = (byte) 0xFF;
														rmcp_rakp3_1[3] = (byte) 0x07;
														rmcp_rakp3_1[4] = (byte) 0x06; // AT/F
																						// 6h
																						// format
																						// =
																						// RMCP+
														rmcp_rakp3_1[5] = (byte) 0x14; // Payload
																						// Type
																						// 1101
																						// 0000
														rmcp_rakp3_1[6] = (byte) 0x00; // IMPI
																						// RMCP+
																						// Session
																						// ID
														rmcp_rakp3_1[7] = (byte) 0x00;
														rmcp_rakp3_1[8] = (byte) 0x00;
														rmcp_rakp3_1[9] = (byte) 0x00;
														rmcp_rakp3_1[10] = (byte) 0x00; // Session
																						// Seq.
																						// No.
														rmcp_rakp3_1[11] = (byte) 0x00;
														rmcp_rakp3_1[12] = (byte) 0x00;
														rmcp_rakp3_1[13] = (byte) 0x00;
														rmcp_rakp3_1[14] = (byte) 0x1c; // IMPI
																						// payload
																						// length
														rmcp_rakp3_1[15] = (byte) 0x00;
														// Msg Tag
														rmcp_rakp3_1[16] = (byte) 0x09;
														rmcp_rakp3_1[17] = (byte) 0x00; // byte
																						// 2:4
																						// -
																						// reserved
														rmcp_rakp3_1[18] = (byte) 0x00;
														rmcp_rakp3_1[19] = (byte) 0x00;

														rmcp_rakp3_1[20] = (byte) inbuffer_osr_1[24];
														rmcp_rakp3_1[21] = (byte) inbuffer_osr_1[25];
														rmcp_rakp3_1[22] = (byte) inbuffer_osr_1[26];
														rmcp_rakp3_1[23] = (byte) inbuffer_osr_1[27];

														rmcp_rakp3_1[24] = (byte) Authcode[0];
														rmcp_rakp3_1[25] = (byte) Authcode[1];
														rmcp_rakp3_1[26] = (byte) Authcode[2];
														rmcp_rakp3_1[27] = (byte) Authcode[3];
														rmcp_rakp3_1[28] = (byte) Authcode[4];
														rmcp_rakp3_1[29] = (byte) Authcode[5];
														rmcp_rakp3_1[30] = (byte) Authcode[6];
														rmcp_rakp3_1[31] = (byte) Authcode[7];
														rmcp_rakp3_1[32] = (byte) Authcode[8];
														rmcp_rakp3_1[33] = (byte) Authcode[9];
														rmcp_rakp3_1[34] = (byte) Authcode[10];
														rmcp_rakp3_1[35] = (byte) Authcode[11];
														rmcp_rakp3_1[36] = (byte) Authcode[12];
														rmcp_rakp3_1[37] = (byte) Authcode[13];
														rmcp_rakp3_1[38] = (byte) Authcode[14];
														rmcp_rakp3_1[39] = (byte) Authcode[15];
														rmcp_rakp3_1[40] = (byte) Authcode[16];
														rmcp_rakp3_1[41] = (byte) Authcode[17];
														rmcp_rakp3_1[42] = (byte) Authcode[18];
														rmcp_rakp3_1[43] = (byte) Authcode[19];

														Packet_rakp3_1 = new DatagramPacket(
																rmcp_rakp3_1,
																rmcp_rakp3_1.length,
																address,
																solPort);

														socket_rakp3_1
																.send(Packet_rakp3_1);

														// 接收
														byte[] inbuffer_rakp3_1 = new byte[256];
														Packet_rakp3_1 = new DatagramPacket(
																inbuffer_rakp3_1,
																inbuffer_rakp3_1.length);
														socket_rakp3_1
																.receive(Packet_rakp3_1);
														socket_rakp3_1.close();

														if (inbuffer_rakp3_1 != null) {
															DatagramPacket Packet_ipmi_1_1;
															DatagramPacket rPacket_ipmi_1_1;
															int ipmi_1_1 = 1;
															try {

																DatagramSocket socket_ipmi_1_1 = new DatagramSocket();
																if (ipmi_1_1 <= 1) {
																	ipmi_1_1++;

																	// ipmi MSG
																	ipmi_msg_1_1[0] = (byte) 0x06;
																	ipmi_msg_1_1[1] = (byte) 0x00;
																	ipmi_msg_1_1[2] = (byte) 0xff;
																	ipmi_msg_1_1[3] = (byte) 0x07;

																	ipmi_msg_1_1[4] = (byte) 0x06;

																	ipmi_msg_1_1[5] = (byte) 0x00;

																	ipmi_msg_1_1[6] = (byte) inbuffer_osr_1[24];
																	ipmi_msg_1_1[7] = (byte) inbuffer_osr_1[25];
																	ipmi_msg_1_1[8] = (byte) inbuffer_osr_1[26];
																	ipmi_msg_1_1[9] = (byte) inbuffer_osr_1[27];

																	ipmi_msg_1_1[10] = (byte) 0x03;// all
																									// 4
																									// bytes
																									// accumulate
																									// 01
																	ipmi_msg_1_1[11] = (byte) 0x00;
																	ipmi_msg_1_1[12] = (byte) 0x00;
																	ipmi_msg_1_1[13] = (byte) 0x00;

																	ipmi_msg_1_1[14] = (byte) 0x08;// message
																									// length
																	ipmi_msg_1_1[15] = (byte) 0x00;

																	ipmi_msg_1_1[16] = (byte) 0x20;
																	ipmi_msg_1_1[17] = (byte) 0x18;
																	ipmi_msg_1_1[18] = (byte) 0xc8;// checksum
																									// 2's
																									// complement

																	ipmi_msg_1_1[19] = (byte) 0x81;// 1
																									// byte
																									// accumulate
																									// 04
																									// (100:bit)
																	ipmi_msg_1_1[20]++;
																	ipmi_msg_1_1[20]++;
																	ipmi_msg_1_1[20]++;
																	ipmi_msg_1_1[20]++;

																	ipmi_msg_1_1[21] = (byte) 0x3b;
																	ipmi_msg_1_1[22] = (byte) 0x04;
																	byte bbb = ((byte) ((byte) ipmi_msg_1_1[19]
																			+ (byte) ipmi_msg_1_1[20]
																			+ (byte) ipmi_msg_1_1[21] + (byte) ipmi_msg_1_1[22]));
																	byte bbbb = (byte) ~bbb;
																	bbbb++;
																	ipmi_msg_1_1[23] = (byte) bbbb;

																	Packet_ipmi_1_1 = new DatagramPacket(
																			ipmi_msg_1_1,
																			ipmi_msg_1_1.length,
																			address,
																			solPort);
																	socket_ipmi_1_1
																			.send(Packet_ipmi_1_1);

																	// 接收
																	byte[] inbuffer_ipmi_1_1 = new byte[256];
																	rPacket_ipmi_1_1 = new DatagramPacket(
																			inbuffer_ipmi_1_1,
																			inbuffer_ipmi_1_1.length);
																	socket_ipmi_1_1
																			.receive(rPacket_ipmi_1_1);
																	socket_ipmi_1_1
																			.close();

																	if (inbuffer_ipmi_1_1 != null) {
																		DatagramPacket Packet_ipmi_2_1;
																		DatagramPacket rPacket_ipmi_2_1;
																		int ipmi_2_1 = 1;
																		try {
																			byte[] ipmi_msg_2_1 = new byte[29];
																			DatagramSocket socket_ipmi_2_1 = new DatagramSocket();
																			if (ipmi_2_1 <= 1) {
																				ipmi_2_1++;
																				ipmi_msg_2_1[0] = (byte) 0x06;
																				ipmi_msg_2_1[1] = (byte) 0x00;
																				ipmi_msg_2_1[2] = (byte) 0xff;
																				ipmi_msg_2_1[3] = (byte) 0x07;

																				ipmi_msg_2_1[4] = (byte) 0x06;

																				ipmi_msg_2_1[5] = (byte) 0x00;

																				ipmi_msg_2_1[6] = (byte) inbuffer_osr_1[24];
																				ipmi_msg_2_1[7] = (byte) inbuffer_osr_1[25];
																				ipmi_msg_2_1[8] = (byte) inbuffer_osr_1[26];
																				ipmi_msg_2_1[9] = (byte) inbuffer_osr_1[27];

																				ipmi_msg_2_1[10] = ++ipmi_msg_1_1[10];// 同步
																				ipmi_msg_2_1[11] = (byte) 0x00;
																				ipmi_msg_2_1[12] = (byte) 0x00;
																				ipmi_msg_2_1[13] = (byte) 0x00;

																				ipmi_msg_2_1[14] = (byte) 0x0D;// message
																												// length
																				ipmi_msg_2_1[15] = (byte) 0x00;

																				ipmi_msg_2_1[16] = (byte) 0x20;
																				ipmi_msg_2_1[17] = (byte) 0x18;
																				ipmi_msg_2_1[18] = (byte) 0xc8;// checksum
																												// 2's
																												// complement

																				ipmi_msg_2_1[19] = (byte) 0x81;// 1
																												// byte
																												// accumulate
																												// 04
																												// (100:bit)
																				ipmi_msg_1_1[20]++;
																				ipmi_msg_1_1[20]++;
																				ipmi_msg_1_1[20]++;
																				ipmi_msg_1_1[20]++;
																				ipmi_msg_2_1[20] = ipmi_msg_1_1[20];// 同1
																				ipmi_msg_2_1[21] = (byte) 0x4c;
																				ipmi_msg_2_1[22] = (byte) 0x01;
																				ipmi_msg_2_1[23] = (byte) 0x02;
																				ipmi_msg_2_1[24] = (byte) 0x02;
																				ipmi_msg_2_1[25] = (byte) 0x00;
																				ipmi_msg_2_1[26] = (byte) 0x00;
																				ipmi_msg_2_1[27] = (byte) 0x00;

																				byte ccc = ((byte) ((byte) ipmi_msg_2_1[19]
																						+ (byte) ipmi_msg_2_1[20]
																						+ (byte) ipmi_msg_2_1[21]
																						+ (byte) ipmi_msg_2_1[22]
																						+ (byte) ipmi_msg_2_1[23]
																						+ (byte) ipmi_msg_2_1[24]
																						+ (byte) ipmi_msg_2_1[25]
																						+ (byte) ipmi_msg_2_1[26] + (byte) ipmi_msg_2_1[27]));
																				byte cccc = (byte) ~ccc;
																				cccc++;
																				ipmi_msg_2_1[28] = (byte) cccc;

																				Packet_ipmi_2_1 = new DatagramPacket(
																						ipmi_msg_2_1,
																						ipmi_msg_2_1.length,
																						address,
																						solPort);
																				socket_ipmi_2_1
																						.send(Packet_ipmi_2_1);
																				byte[] inbuffer_ipmi_2_1 = new byte[256];
																				rPacket_ipmi_2_1 = new DatagramPacket(
																						inbuffer_ipmi_2_1,
																						inbuffer_ipmi_2_1.length);
																				socket_ipmi_2_1
																						.receive(rPacket_ipmi_2_1);
																				socket_ipmi_2_1
																						.close();
																				if (inbuffer_ipmi_2_1 != null) {
																					DatagramPacket Packet_ipmi_3_1;
																					DatagramPacket rPacket_ipmi_3_1;
																					int ipmi_3_1 = 1;
																					try {
																						byte[] ipmi_msg_3_1 = new byte[27];
																						DatagramSocket socket_ipmi_3_1 = new DatagramSocket();
																						if (ipmi_3_1 <= 1) {
																							ipmi_3_1++;
																							ipmi_msg_3_1[0] = (byte) 0x06;
																							ipmi_msg_3_1[1] = (byte) 0x00;
																							ipmi_msg_3_1[2] = (byte) 0xff;
																							ipmi_msg_3_1[3] = (byte) 0x07;

																							ipmi_msg_3_1[4] = (byte) 0x06;

																							ipmi_msg_3_1[5] = (byte) 0x00;

																							ipmi_msg_3_1[6] = (byte) inbuffer_osr_1[24];
																							ipmi_msg_3_1[7] = (byte) inbuffer_osr_1[25];
																							ipmi_msg_3_1[8] = (byte) inbuffer_osr_1[26];
																							ipmi_msg_3_1[9] = (byte) inbuffer_osr_1[27];

																							ipmi_msg_3_1[10] = ++ipmi_msg_1_1[10];// 同步
																							ipmi_msg_3_1[11] = (byte) 0x00;
																							ipmi_msg_3_1[12] = (byte) 0x00;
																							ipmi_msg_3_1[13] = (byte) 0x00;

																							ipmi_msg_3_1[14] = (byte) 0x0b;// message
																															// length
																							ipmi_msg_3_1[15] = (byte) 0x00;

																							ipmi_msg_3_1[16] = (byte) 0x20;
																							ipmi_msg_3_1[17] = (byte) 0x18;
																							ipmi_msg_3_1[18] = (byte) 0xc8;// checksum
																															// 2's
																															// complement

																							ipmi_msg_3_1[19] = (byte) 0x81;// 1
																															// byte
																															// accumulate
																															// 04
																															// (100:bit)
																							ipmi_msg_1_1[20]++;
																							ipmi_msg_1_1[20]++;
																							ipmi_msg_1_1[20]++;
																							ipmi_msg_1_1[20]++;
																							ipmi_msg_3_1[20] = ipmi_msg_1_1[20];// 同1
																							ipmi_msg_3_1[21] = (byte) 0x3c;
																							ipmi_msg_3_1[22] = (byte) inbuffer_osr_1[24];
																							ipmi_msg_3_1[23] = (byte) inbuffer_osr_1[25];
																							ipmi_msg_3_1[24] = (byte) inbuffer_osr_1[26];
																							ipmi_msg_3_1[25] = (byte) inbuffer_osr_1[27];

																							byte ccc1 = ((byte) ((byte) ipmi_msg_3_1[19]
																									+ (byte) ipmi_msg_3_1[20]
																									+ (byte) ipmi_msg_3_1[21]
																									+ (byte) ipmi_msg_3_1[22]
																									+ (byte) ipmi_msg_3_1[23]
																									+ (byte) ipmi_msg_3_1[24] + (byte) ipmi_msg_3_1[25]));
																							byte cccc1 = (byte) ~ccc1;
																							cccc1++;
																							ipmi_msg_3_1[26] = (byte) cccc1;

																							Packet_ipmi_3_1 = new DatagramPacket(
																									ipmi_msg_3_1,
																									ipmi_msg_3_1.length,
																									address,
																									solPort);
																							socket_ipmi_3_1
																									.send(Packet_ipmi_3_1);
																							byte[] inbuffer_ipmi_3_1 = new byte[256];
																							rPacket_ipmi_3_1 = new DatagramPacket(
																									inbuffer_ipmi_3_1,
																									inbuffer_ipmi_3_1.length);
																							socket_ipmi_3_1
																									.receive(rPacket_ipmi_3_1);
																							socket_ipmi_3_1
																									.close();

																							if (inbuffer_ipmi_3_1 != null) {

																								DatagramPacket Packet_osr_2;

																								int osr_2 = 1;
																								try {

																									DatagramSocket socket_osr_2 = new DatagramSocket();

																									if (osr_2 <= 1) {
																										osr_2++;

																										byte[] rmcp_osr_2 = new byte[48];

																										// Op
																										// Session
																										// Req
																										rmcp_osr_2[0] = (byte) 0x06;
																										rmcp_osr_2[1] = (byte) 0x00;
																										rmcp_osr_2[2] = (byte) 0xFF;
																										rmcp_osr_2[3] = (byte) 0x07;

																										rmcp_osr_2[4] = (byte) 0x06;
																										rmcp_osr_2[5] = (byte) 0x10;

																										rmcp_osr_2[6] = (byte) 0x00;
																										rmcp_osr_2[7] = (byte) 0x00;
																										rmcp_osr_2[8] = (byte) 0x00;
																										rmcp_osr_2[9] = (byte) 0x00;

																										rmcp_osr_2[10] = (byte) 0x00;
																										rmcp_osr_2[11] = (byte) 0x00;
																										rmcp_osr_2[12] = (byte) 0x00;
																										rmcp_osr_2[13] = (byte) 0x00;

																										rmcp_osr_2[14] = (byte) 0x20;
																										rmcp_osr_2[15] = (byte) 0x00;

																										rmcp_osr_2[16] = (byte) 0x07;
																										rmcp_osr_2[17] = (byte) 0x04;
																										rmcp_osr_2[18] = (byte) 0x00;
																										rmcp_osr_2[19] = (byte) 0x00;

																										// Remote
																										// Console
																										// Session
																										// ID
																										rmcp_osr_2[20] = (byte) 0xa4;
																										rmcp_osr_2[21] = (byte) 0xa3;

																										rmcp_osr_2[22] = (byte) 0xa2;
																										rmcp_osr_2[23] = (byte) 0xa1;

																										rmcp_osr_2[24] = (byte) 0x00;
																										rmcp_osr_2[25] = (byte) 0x00;
																										rmcp_osr_2[26] = (byte) 0x00;
																										rmcp_osr_2[27] = (byte) 0x08;
																										rmcp_osr_2[28] = (byte) 0x01;
																										rmcp_osr_2[29] = (byte) 0x00;
																										rmcp_osr_2[30] = (byte) 0x00;
																										rmcp_osr_2[31] = (byte) 0x00;

																										rmcp_osr_2[32] = (byte) 0x01;
																										rmcp_osr_2[33] = (byte) 0x00;
																										rmcp_osr_2[34] = (byte) 0x00;
																										rmcp_osr_2[35] = (byte) 0x08;
																										rmcp_osr_2[36] = (byte) 0x00;
																										rmcp_osr_2[37] = (byte) 0x00;
																										rmcp_osr_2[38] = (byte) 0x00;
																										rmcp_osr_2[39] = (byte) 0x00;

																										rmcp_osr_2[40] = (byte) 0x02;
																										rmcp_osr_2[41] = (byte) 0x00;
																										rmcp_osr_2[42] = (byte) 0x00;
																										rmcp_osr_2[43] = (byte) 0x08;
																										rmcp_osr_2[44] = (byte) 0x00;
																										rmcp_osr_2[45] = (byte) 0x00;
																										rmcp_osr_2[46] = (byte) 0x00;
																										rmcp_osr_2[47] = (byte) 0x00;

																										Packet_osr_2 = new DatagramPacket(
																												rmcp_osr_2,
																												rmcp_osr_2.length,
																												address,
																												solPort);

																										socket_osr_2
																												.send(Packet_osr_2);

																										// 接收

																										Packet_osr_2 = new DatagramPacket(
																												inbuffer_osr_2,
																												inbuffer_osr_2.length);
																										socket_osr_2
																												.receive(Packet_osr_2);
																										socket_osr_2
																												.close();

																										if (inbuffer_osr_2 != null) {

																											DatagramPacket Packet_rakp1_2;

																											int rakp1_2 = 1;
																											try {

																												DatagramSocket socket_rakp1_2 = new DatagramSocket();

																												if (rakp1_2 <= 1) {
																													rakp1_2++;

																													byte[] rmcp_rakp1_2 = new byte[60];

																													// RAKP
																													// Msg
																													// 1
																													rmcp_rakp1_2[0] = (byte) 0x06; // IMPI
																																					// header
																													rmcp_rakp1_2[1] = (byte) 0x00;
																													rmcp_rakp1_2[2] = (byte) 0xFF;
																													rmcp_rakp1_2[3] = (byte) 0x07;
																													rmcp_rakp1_2[4] = (byte) 0x06; // AT/F
																																					// 6h
																																					// format
																																					// =
																																					// RMCP+
																													rmcp_rakp1_2[5] = (byte) 0x12; // Payload
																																					// Type
																																					// 1101
																																					// 0000
																													rmcp_rakp1_2[6] = (byte) 0x00; // IMPI
																																					// RMCP+
																																					// Session
																																					// ID
																													rmcp_rakp1_2[7] = (byte) 0x00;
																													rmcp_rakp1_2[8] = (byte) 0x00;
																													rmcp_rakp1_2[9] = (byte) 0x00;
																													rmcp_rakp1_2[10] = (byte) 0x00; // Session
																																					// Seq.
																																					// No.
																													rmcp_rakp1_2[11] = (byte) 0x00;
																													rmcp_rakp1_2[12] = (byte) 0x00;
																													rmcp_rakp1_2[13] = (byte) 0x00;
																													rmcp_rakp1_2[14] = (byte) 0x2C; // IMPI
																																					// payload
																																					// length
																													rmcp_rakp1_2[15] = (byte) 0x00;
																													// Msg
																													// Tag
																													rmcp_rakp1_2[16] = (byte) 0x08;
																													rmcp_rakp1_2[17] = (byte) 0x00; // byte
																																					// 2:4
																																					// -
																																					// reserved
																													rmcp_rakp1_2[18] = (byte) 0x00;
																													rmcp_rakp1_2[19] = (byte) 0x00;

																													rmcp_rakp1_2[20] = (byte) inbuffer_osr_2[24];
																													rmcp_rakp1_2[21] = (byte) inbuffer_osr_2[25];
																													rmcp_rakp1_2[22] = (byte) inbuffer_osr_2[26];
																													rmcp_rakp1_2[23] = (byte) inbuffer_osr_2[27];

																													// Remote
																													// Console
																													// Random
																													// Number,自己產生
																													rmcp_rakp1_2[24] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[25] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[26] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[27] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[28] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[29] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[30] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[31] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[32] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[33] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[34] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[35] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[36] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[37] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[38] = (byte) randomG
																															.nextInt(128);
																													rmcp_rakp1_2[39] = (byte) randomG
																															.nextInt(128);

																													rmcp_rakp1_2[40] = (byte) userPriv; // level

																													rmcp_rakp1_2[41] = (byte) 0x00; // byte
																																					// 2:4
																																					// -
																																					// reserved
																													rmcp_rakp1_2[42] = (byte) 0x00;

																													rmcp_rakp1_2[43] = (byte) ID_length();// User
																																							// Name
																																							// Length

																													rmcp_rakp1_2[44] = (byte) 0x00;
																													rmcp_rakp1_2[45] = (byte) 0x00;
																													rmcp_rakp1_2[46] = (byte) 0x00;
																													rmcp_rakp1_2[47] = (byte) 0x00;
																													rmcp_rakp1_2[48] = (byte) 0x00;
																													rmcp_rakp1_2[49] = (byte) 0x00;
																													rmcp_rakp1_2[50] = (byte) 0x00;
																													rmcp_rakp1_2[51] = (byte) 0x00;
																													rmcp_rakp1_2[52] = (byte) 0x00;
																													rmcp_rakp1_2[53] = (byte) 0x00;
																													rmcp_rakp1_2[54] = (byte) 0x00;
																													rmcp_rakp1_2[55] = (byte) 0x00;
																													rmcp_rakp1_2[56] = (byte) 0x00;
																													rmcp_rakp1_2[57] = (byte) 0x00;
																													rmcp_rakp1_2[58] = (byte) 0x00;
																													rmcp_rakp1_2[59] = (byte) 0x00;

																													int id_1 = 0;
																													for (id_1 = 0; id_1 < ID_length_int(); id_1++) {
																														rmcp_rakp1_2[44 + id_1] = (byte) id_byte[id_1];
																													}

																													Packet_rakp1_2 = new DatagramPacket(
																															rmcp_rakp1_2,
																															rmcp_rakp1_2.length,
																															address,
																															solPort);

																													socket_rakp1_2
																															.send(Packet_rakp1_2);

																													// 接收
																													byte[] inbuffer_rakp1_2 = new byte[256];
																													Packet_rakp1_2 = new DatagramPacket(
																															inbuffer_rakp1_2,
																															inbuffer_rakp1_2.length);
																													socket_rakp1_2
																															.receive(Packet_rakp1_2);
																													socket_rakp1_2
																															.close();
																													if (inbuffer_rakp1_2 != null)

																													{

																														DatagramPacket Packet_rakp3_2;

																														int rakp3_2 = 1;
																														try {

																															DatagramSocket socket_rakp3_2 = new DatagramSocket();

																															if (rakp3_2 <= 1) {
																																rakp3_2++;

																																// Data
																																// and
																																// Keys
																																// to
																																// Auth
																																// code
																																byte[] Data2 = new byte[40];
																																byte[] Key2 = new byte[25];
																																Data2[0] = (byte) inbuffer_rakp1_2[24];
																																Data2[1] = (byte) inbuffer_rakp1_2[25];
																																Data2[2] = (byte) inbuffer_rakp1_2[26];
																																Data2[3] = (byte) inbuffer_rakp1_2[27];
																																Data2[4] = (byte) inbuffer_rakp1_2[28];
																																Data2[5] = (byte) inbuffer_rakp1_2[29];
																																Data2[6] = (byte) inbuffer_rakp1_2[30];
																																Data2[7] = (byte) inbuffer_rakp1_2[31];
																																Data2[8] = (byte) inbuffer_rakp1_2[32];
																																Data2[9] = (byte) inbuffer_rakp1_2[33];
																																Data2[10] = (byte) inbuffer_rakp1_2[34];
																																Data2[11] = (byte) inbuffer_rakp1_2[35];
																																Data2[12] = (byte) inbuffer_rakp1_2[36];
																																Data2[13] = (byte) inbuffer_rakp1_2[37];
																																Data2[14] = (byte) inbuffer_rakp1_2[38];
																																Data2[15] = (byte) inbuffer_rakp1_2[39];
																																Data2[16] = (byte) inbuffer_rakp1_2[20];
																																Data2[17] = (byte) inbuffer_rakp1_2[21];
																																Data2[18] = (byte) inbuffer_rakp1_2[22];
																																Data2[19] = (byte) inbuffer_rakp1_2[23];
																																Data2[20] = (byte) userPriv;
																																Data2[21] = (byte) ID_length();

																																byte pw_byte2[] = siteField_PW
																																		.getText()
																																		.getBytes();
																																byte id_byte2[] = siteField_ID
																																		.getText()
																																		.getBytes();

																																Data2[22] = (byte) 0x00;
																																Data2[23] = (byte) 0x00;
																																Data2[24] = (byte) 0x00;
																																Data2[25] = (byte) 0x00;
																																Data2[26] = (byte) 0x00;
																																Data2[27] = (byte) 0x00;
																																Data2[28] = (byte) 0x00;
																																Data2[29] = (byte) 0x00;
																																Data2[30] = (byte) 0x00;
																																Data2[31] = (byte) 0x00;
																																Data2[32] = (byte) 0x00;
																																Data2[33] = (byte) 0x00;
																																Data2[34] = (byte) 0x00;
																																Data2[35] = (byte) 0x00;
																																Data2[36] = (byte) 0x00;
																																Data2[37] = (byte) 0x00;
																																Data2[38] = (byte) 0x00;
																																Data2[39] = (byte) 0x00;

																																int pw2 = 0;
																																for (pw2 = 0; pw2 < PW_length_int(); pw2++) {
																																	Key2[pw2] = (byte) pw_byte2[pw2];
																																}

																																for (pw2 = 0; pw2 < ID_length_int(); pw2++) {
																																	Data2[22 + pw2] = (byte) id_byte2[pw2];
																																}
																																int Key_Length2 = PW_length_int();
																																int ID_Length2 = ID_length_int();
																																int Data_Length2 = 22 + ID_Length2;

																																byte[] Authcode2 = HMACSHA1(
																																		Data2,
																																		Data_Length2,
																																		Key2,
																																		Key_Length2);

																																byte[] rmcp_rakp3_2 = new byte[44];

																																// RAKP
																																// Msg
																																// 3
																																rmcp_rakp3_2[0] = (byte) 0x06; // IMPI
																																								// header
																																rmcp_rakp3_2[1] = (byte) 0x00;
																																rmcp_rakp3_2[2] = (byte) 0xFF;
																																rmcp_rakp3_2[3] = (byte) 0x07;
																																rmcp_rakp3_2[4] = (byte) 0x06; // AT/F
																																								// 6h
																																								// format
																																								// =
																																								// RMCP+
																																rmcp_rakp3_2[5] = (byte) 0x14; // Payload
																																								// Type
																																								// 1101
																																								// 0000
																																rmcp_rakp3_2[6] = (byte) 0x00; // IMPI
																																								// RMCP+
																																								// Session
																																								// ID
																																rmcp_rakp3_2[7] = (byte) 0x00;
																																rmcp_rakp3_2[8] = (byte) 0x00;
																																rmcp_rakp3_2[9] = (byte) 0x00;
																																rmcp_rakp3_2[10] = (byte) 0x00; // Session
																																								// Seq.
																																								// No.
																																rmcp_rakp3_2[11] = (byte) 0x00;
																																rmcp_rakp3_2[12] = (byte) 0x00;
																																rmcp_rakp3_2[13] = (byte) 0x00;
																																rmcp_rakp3_2[14] = (byte) 0x1C; // IMPI
																																								// payload
																																								// length
																																rmcp_rakp3_2[15] = (byte) 0x00;
																																// Msg
																																// Tag
																																rmcp_rakp3_2[16] = (byte) 0x09;
																																rmcp_rakp3_2[17] = (byte) 0x00; // byte
																																								// 2:4
																																								// -
																																								// reserved
																																rmcp_rakp3_2[18] = (byte) 0x00;
																																rmcp_rakp3_2[19] = (byte) 0x00;

																																rmcp_rakp3_2[20] = (byte) inbuffer_osr_2[24];
																																rmcp_rakp3_2[21] = (byte) inbuffer_osr_2[25];
																																rmcp_rakp3_2[22] = (byte) inbuffer_osr_2[26];
																																rmcp_rakp3_2[23] = (byte) inbuffer_osr_2[27];

																																rmcp_rakp3_2[24] = (byte) Authcode2[0];
																																rmcp_rakp3_2[25] = (byte) Authcode2[1];
																																rmcp_rakp3_2[26] = (byte) Authcode2[2];
																																rmcp_rakp3_2[27] = (byte) Authcode2[3];
																																rmcp_rakp3_2[28] = (byte) Authcode2[4];
																																rmcp_rakp3_2[29] = (byte) Authcode2[5];
																																rmcp_rakp3_2[30] = (byte) Authcode2[6];
																																rmcp_rakp3_2[31] = (byte) Authcode2[7];
																																rmcp_rakp3_2[32] = (byte) Authcode2[8];
																																rmcp_rakp3_2[33] = (byte) Authcode2[9];
																																rmcp_rakp3_2[34] = (byte) Authcode2[10];
																																rmcp_rakp3_2[35] = (byte) Authcode2[11];
																																rmcp_rakp3_2[36] = (byte) Authcode2[12];
																																rmcp_rakp3_2[37] = (byte) Authcode2[13];
																																rmcp_rakp3_2[38] = (byte) Authcode2[14];
																																rmcp_rakp3_2[39] = (byte) Authcode2[15];
																																rmcp_rakp3_2[40] = (byte) Authcode2[16];
																																rmcp_rakp3_2[41] = (byte) Authcode2[17];
																																rmcp_rakp3_2[42] = (byte) Authcode2[18];
																																rmcp_rakp3_2[43] = (byte) Authcode2[19];

																																Packet_rakp3_2 = new DatagramPacket(
																																		rmcp_rakp3_2,
																																		rmcp_rakp3_2.length,
																																		address,
																																		solPort);

																																socket_rakp3_2
																																		.send(Packet_rakp3_2);

																																// 接收
																																byte[] inbuffer_rakp3_2 = new byte[256];
																																Packet_rakp3_2 = new DatagramPacket(
																																		inbuffer_rakp3_2,
																																		inbuffer_rakp3_2.length);
																																socket_rakp3_2
																																		.receive(Packet_rakp3_2);
																																socket_rakp3_2
																																		.close();

																																if (inbuffer_rakp3_2 != null) {
																																	DatagramPacket Packet_ipmi_1_2;
																																	DatagramPacket rPacket_ipmi_1_2;
																																	int ipmi_1_2 = 1;
																																	try {

																																		DatagramSocket socket_ipmi_1_2 = new DatagramSocket();
																																		if (ipmi_1_2 <= 1) {
																																			ipmi_1_2++;

																																			// ipmi
																																			// MSG
																																			ipmi_msg_1_2[0] = (byte) 0x06;
																																			ipmi_msg_1_2[1] = (byte) 0x00;
																																			ipmi_msg_1_2[2] = (byte) 0xff;
																																			ipmi_msg_1_2[3] = (byte) 0x07;

																																			ipmi_msg_1_2[4] = (byte) 0x06;

																																			ipmi_msg_1_2[5] = (byte) 0x00;

																																			ipmi_msg_1_2[6] = (byte) inbuffer_osr_2[24];
																																			ipmi_msg_1_2[7] = (byte) inbuffer_osr_2[25];
																																			ipmi_msg_1_2[8] = (byte) inbuffer_osr_2[26];
																																			ipmi_msg_1_2[9] = (byte) inbuffer_osr_2[27];

																																			ipmi_msg_1_2[10] = (byte) 0x03;// all
																																											// 4
																																											// bytes
																																											// accumulate
																																											// 01
																																			ipmi_msg_1_2[11] = (byte) 0x00;
																																			ipmi_msg_1_2[12] = (byte) 0x00;
																																			ipmi_msg_1_2[13] = (byte) 0x00;

																																			ipmi_msg_1_2[14] = (byte) 0x08;// message
																																											// length
																																			ipmi_msg_1_2[15] = (byte) 0x00;

																																			ipmi_msg_1_2[16] = (byte) 0x20;
																																			ipmi_msg_1_2[17] = (byte) 0x18;
																																			ipmi_msg_1_2[18] = (byte) 0xc8;// checksum
																																											// 2's
																																											// complement

																																			ipmi_msg_1_2[19] = (byte) 0x81;// 1
																																											// byte
																																											// accumulate
																																											// 04
																																											// (100:bit)
																																			ipmi_msg_1_2[20]++;
																																			ipmi_msg_1_2[20]++;
																																			ipmi_msg_1_2[20]++;
																																			ipmi_msg_1_2[20]++;

																																			ipmi_msg_1_2[21] = (byte) 0x3b;
																																			ipmi_msg_1_2[22] = (byte) 0x04;
																																			byte bbb_1 = ((byte) ((byte) ipmi_msg_1_2[19]
																																					+ (byte) ipmi_msg_1_2[20]
																																					+ (byte) ipmi_msg_1_2[21] + (byte) ipmi_msg_1_2[22]));
																																			byte bbbb_1 = (byte) ~bbb_1;
																																			bbbb_1++;
																																			ipmi_msg_1_2[23] = (byte) bbbb_1;

																																			Packet_ipmi_1_2 = new DatagramPacket(
																																					ipmi_msg_1_2,
																																					ipmi_msg_1_2.length,
																																					address,
																																					solPort);
																																			socket_ipmi_1_2
																																					.send(Packet_ipmi_1_2);

																																			// 接收
																																			byte[] inbuffer_ipmi_1_2 = new byte[256];
																																			rPacket_ipmi_1_2 = new DatagramPacket(
																																					inbuffer_ipmi_1_2,
																																					inbuffer_ipmi_1_2.length);
																																			socket_ipmi_1_2
																																					.receive(rPacket_ipmi_1_2);

																																			socket_rakp1_2
																																					.close();
																																			if (inbuffer_ipmi_1_2 != null) {
																																				DatagramPacket Packet_ipmi_2_2;
																																				DatagramPacket rPacket_ipmi_2_2;
																																				int ipmi_2_2 = 1;
																																				try {
																																					byte[] ipmi_msg_2_2 = new byte[26];

																																					DatagramSocket socket_ipmi_2_2 = new DatagramSocket();
																																					if (ipmi_2_2 <= 1) {
																																						ipmi_2_2++;
																																						ipmi_msg_2_2[0] = (byte) 0x06;
																																						ipmi_msg_2_2[1] = (byte) 0x00;
																																						ipmi_msg_2_2[2] = (byte) 0xff;
																																						ipmi_msg_2_2[3] = (byte) 0x07;

																																						ipmi_msg_2_2[4] = (byte) 0x06;

																																						ipmi_msg_2_2[5] = (byte) 0x00;

																																						ipmi_msg_2_2[6] = (byte) inbuffer_osr_2[24];
																																						ipmi_msg_2_2[7] = (byte) inbuffer_osr_2[25];
																																						ipmi_msg_2_2[8] = (byte) inbuffer_osr_2[26];
																																						ipmi_msg_2_2[9] = (byte) inbuffer_osr_2[27];

																																						ipmi_msg_2_2[10] = ++ipmi_msg_1_2[10];// 同步
																																						ipmi_msg_2_2[11] = (byte) 0x00;
																																						ipmi_msg_2_2[12] = (byte) 0x00;
																																						ipmi_msg_2_2[13] = (byte) 0x00;

																																						ipmi_msg_2_2[14] = (byte) 0x0a;// message
																																														// length
																																						ipmi_msg_2_2[15] = (byte) 0x00;

																																						ipmi_msg_2_2[16] = (byte) 0x20;
																																						ipmi_msg_2_2[17] = (byte) 0x30;
																																						ipmi_msg_2_2[18] = (byte) 0xb0;// checksum
																																														// 2's
																																														// complement

																																						ipmi_msg_2_2[19] = (byte) 0x81;// 1
																																														// byte
																																														// accumulate
																																														// 04
																																														// (100:bit)
																																						ipmi_msg_1_2[20]++;
																																						ipmi_msg_1_2[20]++;
																																						ipmi_msg_1_2[20]++;
																																						ipmi_msg_1_2[20]++;
																																						ipmi_msg_2_2[20] = ipmi_msg_1_2[20];// 同1
																																						ipmi_msg_2_2[21] = (byte) 0x21;
																																						ipmi_msg_2_2[22] = (byte) 0x0e;
																																						ipmi_msg_2_2[23] = (byte) 0x00;
																																						ipmi_msg_2_2[24] = (byte) 0x01;

																																						byte ccc_1 = ((byte) ((byte) ipmi_msg_2_2[19]
																																								+ (byte) ipmi_msg_2_2[20]
																																								+ (byte) ipmi_msg_2_2[21]
																																								+ (byte) ipmi_msg_2_2[22]
																																								+ (byte) ipmi_msg_2_2[23] + (byte) ipmi_msg_2_2[24]));
																																						byte cccc_1 = (byte) ~ccc_1;
																																						cccc_1++;
																																						ipmi_msg_2_2[25] = (byte) cccc_1;

																																						Packet_ipmi_2_2 = new DatagramPacket(
																																								ipmi_msg_2_2,
																																								ipmi_msg_2_2.length,
																																								address,
																																								solPort);
																																						socket_ipmi_2_2
																																								.send(Packet_ipmi_2_2);
																																						byte[] inbuffer_ipmi_2_2 = new byte[256];
																																						rPacket_ipmi_2_2 = new DatagramPacket(
																																								inbuffer_ipmi_2_2,
																																								inbuffer_ipmi_2_2.length);
																																						socket_ipmi_2_2
																																								.receive(rPacket_ipmi_2_2);
																																						socket_ipmi_2_2
																																								.close();
																																						if (inbuffer_ipmi_2_2 != null) {
																																							DatagramPacket Packet_ipmi_3_2;
																																							DatagramPacket rPacket_ipmi_3_2;
																																							int ipmi_3_2 = 1;
																																							try {
																																								byte[] ipmi_msg_3_2 = new byte[26];

																																								DatagramSocket socket_ipmi_3_2 = new DatagramSocket();
																																								if (ipmi_3_2 <= 1) {
																																									ipmi_3_2++;
																																									ipmi_msg_3_2[0] = (byte) 0x06;
																																									ipmi_msg_3_2[1] = (byte) 0x00;
																																									ipmi_msg_3_2[2] = (byte) 0xff;
																																									ipmi_msg_3_2[3] = (byte) 0x07;

																																									ipmi_msg_3_2[4] = (byte) 0x06;

																																									ipmi_msg_3_2[5] = (byte) 0x00;

																																									ipmi_msg_3_2[6] = (byte) inbuffer_osr_2[24];
																																									ipmi_msg_3_2[7] = (byte) inbuffer_osr_2[25];
																																									ipmi_msg_3_2[8] = (byte) inbuffer_osr_2[26];
																																									ipmi_msg_3_2[9] = (byte) inbuffer_osr_2[27];

																																									ipmi_msg_3_2[10] = ++ipmi_msg_1_2[10];// 同步
																																									ipmi_msg_3_2[11] = (byte) 0x00;
																																									ipmi_msg_3_2[12] = (byte) 0x00;
																																									ipmi_msg_3_2[13] = (byte) 0x00;

																																									ipmi_msg_3_2[14] = (byte) 0x0a;// message
																																																	// length
																																									ipmi_msg_3_2[15] = (byte) 0x00;

																																									ipmi_msg_3_2[16] = (byte) 0x20;
																																									ipmi_msg_3_2[17] = (byte) 0x30;
																																									ipmi_msg_3_2[18] = (byte) 0xb0;// checksum
																																																	// 2's
																																																	// complement

																																									ipmi_msg_3_2[19] = (byte) 0x81;// 1
																																																	// byte
																																																	// accumulate
																																																	// 04
																																																	// (100:bit)
																																									ipmi_msg_1_2[20]++;
																																									ipmi_msg_1_2[20]++;
																																									ipmi_msg_1_2[20]++;
																																									ipmi_msg_1_2[20]++;
																																									ipmi_msg_3_2[20] = ipmi_msg_1_2[20];// 同1
																																									ipmi_msg_3_2[21] = (byte) 0x21;
																																									ipmi_msg_3_2[22] = (byte) 0x0e;
																																									ipmi_msg_3_2[23] = (byte) 0x01;
																																									ipmi_msg_3_2[24] = (byte) 0x01;

																																									byte ddd1_1 = ((byte) ((byte) ipmi_msg_3_2[19]
																																											+ (byte) ipmi_msg_3_2[20]
																																											+ (byte) ipmi_msg_3_2[21]
																																											+ (byte) ipmi_msg_3_2[22]
																																											+ (byte) ipmi_msg_3_2[23] + (byte) ipmi_msg_3_2[24]));
																																									byte dddd1_1 = (byte) ~ddd1_1;
																																									dddd1_1++;
																																									ipmi_msg_3_2[25] = (byte) dddd1_1;

																																									Packet_ipmi_3_2 = new DatagramPacket(
																																											ipmi_msg_3_2,
																																											ipmi_msg_3_2.length,
																																											address,
																																											solPort);
																																									socket_ipmi_3_2
																																											.send(Packet_ipmi_3_2);
																																									byte[] inbuffer_ipmi_3_2 = new byte[256];
																																									rPacket_ipmi_3_2 = new DatagramPacket(
																																											inbuffer_ipmi_3_2,
																																											inbuffer_ipmi_3_2.length);
																																									socket_ipmi_3_2
																																											.receive(rPacket_ipmi_3_2);

																																									socket_ipmi_3_2
																																											.close();

																																									if (inbuffer_ipmi_3_2 != null) {
																																										DatagramPacket Packet_ipmi_4_2;
																																										DatagramPacket rPacket_ipmi_4_2;
																																										int ipmi_4_2 = 1;
																																										try {
																																											byte[] ipmi_msg_4_2 = new byte[26];

																																											DatagramSocket socket_ipmi_4_2 = new DatagramSocket();
																																											if (ipmi_4_2 <= 1) {
																																												ipmi_4_2++;
																																												ipmi_msg_4_2[0] = (byte) 0x06;
																																												ipmi_msg_4_2[1] = (byte) 0x00;
																																												ipmi_msg_4_2[2] = (byte) 0xff;
																																												ipmi_msg_4_2[3] = (byte) 0x07;

																																												ipmi_msg_4_2[4] = (byte) 0x06;

																																												ipmi_msg_4_2[5] = (byte) 0x00;

																																												ipmi_msg_4_2[6] = (byte) inbuffer_osr_2[24];
																																												ipmi_msg_4_2[7] = (byte) inbuffer_osr_2[25];
																																												ipmi_msg_4_2[8] = (byte) inbuffer_osr_2[26];
																																												ipmi_msg_4_2[9] = (byte) inbuffer_osr_2[27];

																																												ipmi_msg_4_2[10] = ++ipmi_msg_1_2[10];// 同步
																																												ipmi_msg_4_2[11] = (byte) 0x00;
																																												ipmi_msg_4_2[12] = (byte) 0x00;
																																												ipmi_msg_4_2[13] = (byte) 0x00;

																																												ipmi_msg_4_2[14] = (byte) 0x0a;// message
																																																				// length
																																												ipmi_msg_4_2[15] = (byte) 0x00;

																																												ipmi_msg_4_2[16] = (byte) 0x20;
																																												ipmi_msg_4_2[17] = (byte) 0x30;
																																												ipmi_msg_4_2[18] = (byte) 0xb0;// checksum
																																																				// 2's
																																																				// complement

																																												ipmi_msg_4_2[19] = (byte) 0x81;// 1
																																																				// byte
																																																				// accumulate
																																																				// 04
																																																				// (100:bit)
																																												ipmi_msg_1_2[20]++;
																																												ipmi_msg_1_2[20]++;
																																												ipmi_msg_1_2[20]++;
																																												ipmi_msg_1_2[20]++;
																																												ipmi_msg_4_2[20] = ipmi_msg_1_2[20];// 同1
																																												ipmi_msg_4_2[21] = (byte) 0x21;
																																												ipmi_msg_4_2[22] = (byte) 0x0e;
																																												ipmi_msg_4_2[23] = (byte) 0x00;
																																												ipmi_msg_4_2[24] = (byte) 0x02;

																																												byte ccc2 = ((byte) ((byte) ipmi_msg_4_2[19]
																																														+ (byte) ipmi_msg_4_2[20]
																																														+ (byte) ipmi_msg_4_2[21]
																																														+ (byte) ipmi_msg_4_2[22]
																																														+ (byte) ipmi_msg_4_2[23] + (byte) ipmi_msg_4_2[24]));
																																												byte cccc2 = (byte) ~ccc2;
																																												cccc2++;
																																												ipmi_msg_4_2[25] = (byte) cccc2;

																																												Packet_ipmi_4_2 = new DatagramPacket(
																																														ipmi_msg_4_2,
																																														ipmi_msg_4_2.length,
																																														address,
																																														solPort);
																																												socket_ipmi_4_2
																																														.send(Packet_ipmi_4_2);
																																												byte[] inbuffer_ipmi_4_2 = new byte[256];
																																												rPacket_ipmi_4_2 = new DatagramPacket(
																																														inbuffer_ipmi_4_2,
																																														inbuffer_ipmi_4_2.length);
																																												socket_ipmi_4_2
																																														.receive(rPacket_ipmi_4_2);

																																												socket_ipmi_4_2
																																														.close();
																																												if (inbuffer_ipmi_4_2 != null) {
																																													DatagramPacket Packet_ipmi_5_2;
																																													DatagramPacket rPacket_ipmi_5_2;
																																													int ipmi_5_2 = 1;
																																													try {
																																														byte[] ipmi_msg_5_2 = new byte[26];

																																														DatagramSocket socket_ipmi_5_2 = new DatagramSocket();
																																														if (ipmi_5_2 <= 1) {
																																															ipmi_5_2++;
																																															ipmi_msg_5_2[0] = (byte) 0x06;
																																															ipmi_msg_5_2[1] = (byte) 0x00;
																																															ipmi_msg_5_2[2] = (byte) 0xff;
																																															ipmi_msg_5_2[3] = (byte) 0x07;

																																															ipmi_msg_5_2[4] = (byte) 0x06;

																																															ipmi_msg_5_2[5] = (byte) 0x00;

																																															ipmi_msg_5_2[6] = (byte) inbuffer_osr_2[24];
																																															ipmi_msg_5_2[7] = (byte) inbuffer_osr_2[25];
																																															ipmi_msg_5_2[8] = (byte) inbuffer_osr_2[26];
																																															ipmi_msg_5_2[9] = (byte) inbuffer_osr_2[27];

																																															ipmi_msg_5_2[10] = ++ipmi_msg_1_2[10];// 同步
																																															ipmi_msg_5_2[11] = (byte) 0x00;
																																															ipmi_msg_5_2[12] = (byte) 0x00;
																																															ipmi_msg_5_2[13] = (byte) 0x00;

																																															ipmi_msg_5_2[14] = (byte) 0x0a;// message
																																																							// length
																																															ipmi_msg_5_2[15] = (byte) 0x00;

																																															ipmi_msg_5_2[16] = (byte) 0x20;
																																															ipmi_msg_5_2[17] = (byte) 0x30;
																																															ipmi_msg_5_2[18] = (byte) 0xb0;// checksum
																																																							// 2's
																																																							// complement

																																															ipmi_msg_5_2[19] = (byte) 0x81;// 1
																																																							// byte
																																																							// accumulate
																																																							// 04
																																																							// (100:bit)
																																															ipmi_msg_1_2[20]++;
																																															ipmi_msg_1_2[20]++;
																																															ipmi_msg_1_2[20]++;
																																															ipmi_msg_1_2[20]++;
																																															ipmi_msg_5_2[20] = ipmi_msg_1_2[20];// 同1
																																															ipmi_msg_5_2[21] = (byte) 0x21;
																																															ipmi_msg_5_2[22] = (byte) 0x0e;
																																															ipmi_msg_5_2[23] = (byte) 0x00;
																																															ipmi_msg_5_2[24] = (byte) 0x00;

																																															byte ccc3 = ((byte) ((byte) ipmi_msg_5_2[19]
																																																	+ (byte) ipmi_msg_5_2[20]
																																																	+ (byte) ipmi_msg_5_2[21]
																																																	+ (byte) ipmi_msg_5_2[22]
																																																	+ (byte) ipmi_msg_5_2[23] + (byte) ipmi_msg_5_2[24]));
																																															byte cccc3 = (byte) ~ccc3;
																																															cccc3++;
																																															ipmi_msg_5_2[25] = (byte) cccc3;

																																															Packet_ipmi_5_2 = new DatagramPacket(
																																																	ipmi_msg_5_2,
																																																	ipmi_msg_5_2.length,
																																																	address,
																																																	solPort);
																																															socket_ipmi_5_2
																																																	.send(Packet_ipmi_5_2);
																																															byte[] inbuffer_ipmi_5_2 = new byte[256];
																																															rPacket_ipmi_5_2 = new DatagramPacket(
																																																	inbuffer_ipmi_5_2,
																																																	inbuffer_ipmi_5_2.length);
																																															socket_ipmi_5_2
																																																	.receive(rPacket_ipmi_5_2);
																																															socket_ipmi_5_2
																																																	.close();
																																															if (inbuffer_ipmi_5_2 != null) {
																																																DatagramPacket Packet_ipmi_6_2;
																																																DatagramPacket rPacket_ipmi_6_2;
																																																int ipmi_6_2 = 1;
																																																try {
																																																	byte[] ipmi_msg_6_2 = new byte[27];

																																																	DatagramSocket socket_ipmi_6_2 = new DatagramSocket();
																																																	if (ipmi_6_2 <= 1) {
																																																		ipmi_6_2++;
																																																		ipmi_msg_6_2[0] = (byte) 0x06;
																																																		ipmi_msg_6_2[1] = (byte) 0x00;
																																																		ipmi_msg_6_2[2] = (byte) 0xff;
																																																		ipmi_msg_6_2[3] = (byte) 0x07;

																																																		ipmi_msg_6_2[4] = (byte) 0x06;

																																																		ipmi_msg_6_2[5] = (byte) 0x00;

																																																		ipmi_msg_6_2[6] = (byte) inbuffer_osr_2[24];
																																																		ipmi_msg_6_2[7] = (byte) inbuffer_osr_2[25];
																																																		ipmi_msg_6_2[8] = (byte) inbuffer_osr_2[26];
																																																		ipmi_msg_6_2[9] = (byte) inbuffer_osr_2[27];

																																																		ipmi_msg_6_2[10] = ++ipmi_msg_1_2[10];// 同步
																																																		ipmi_msg_6_2[11] = (byte) 0x00;
																																																		ipmi_msg_6_2[12] = (byte) 0x00;
																																																		ipmi_msg_6_2[13] = (byte) 0x00;

																																																		ipmi_msg_6_2[14] = (byte) 0x0b;// message
																																																										// length
																																																		ipmi_msg_6_2[15] = (byte) 0x00;

																																																		ipmi_msg_6_2[16] = (byte) 0x20;
																																																		ipmi_msg_6_2[17] = (byte) 0x18;
																																																		ipmi_msg_6_2[18] = (byte) 0xc8;// checksum
																																																										// 2's
																																																										// complement

																																																		ipmi_msg_6_2[19] = (byte) 0x81;// 1
																																																										// byte
																																																										// accumulate
																																																										// 04
																																																										// (100:bit)
																																																		ipmi_msg_1_2[20]++;
																																																		ipmi_msg_1_2[20]++;
																																																		ipmi_msg_1_2[20]++;
																																																		ipmi_msg_1_2[20]++;
																																																		ipmi_msg_6_2[20] = ipmi_msg_1_2[20];// 同1
																																																		ipmi_msg_6_2[21] = (byte) 0x3c;
																																																		ipmi_msg_6_2[22] = (byte) inbuffer_osr_2[24];
																																																		ipmi_msg_6_2[23] = (byte) inbuffer_osr_2[25];
																																																		ipmi_msg_6_2[24] = (byte) inbuffer_osr_2[26];
																																																		ipmi_msg_6_2[25] = (byte) inbuffer_osr_2[27];

																																																		byte ccc1_1 = ((byte) ((byte) ipmi_msg_6_2[19]
																																																				+ (byte) ipmi_msg_6_2[20]
																																																				+ (byte) ipmi_msg_6_2[21]
																																																				+ (byte) ipmi_msg_6_2[22]
																																																				+ (byte) ipmi_msg_6_2[23]
																																																				+ (byte) ipmi_msg_6_2[24] + (byte) ipmi_msg_6_2[25]));
																																																		byte cccc1_1 = (byte) ~ccc1_1;
																																																		cccc1_1++;
																																																		ipmi_msg_6_2[26] = (byte) cccc1_1;

																																																		Packet_ipmi_6_2 = new DatagramPacket(
																																																				ipmi_msg_6_2,
																																																				ipmi_msg_6_2.length,
																																																				address,
																																																				solPort);
																																																		socket_ipmi_6_2
																																																				.send(Packet_ipmi_6_2);
																																																		byte[] inbuffer_ipmi_6_2 = new byte[256];
																																																		rPacket_ipmi_6_2 = new DatagramPacket(
																																																				inbuffer_ipmi_6_2,
																																																				inbuffer_ipmi_6_2.length);
																																																		socket_ipmi_6_2
																																																				.receive(rPacket_ipmi_6_2);
																																																		socket_ipmi_6_2
																																																				.close();

																																																		if (inbuffer_ipmi_6_2 != null) {

																																																			DatagramPacket Packet_gcac;
																																																			Random randomG_1 = new Random();

																																																			int gcac = 1;
																																																			try {

																																																				DatagramSocket socket_gcac = new DatagramSocket();

																																																				if (gcac <= 1) {
																																																					gcac++;

																																																					byte[] rmcp_gcac = new byte[23];

																																																					// Get
																																																					// Cha.
																																																					// A.C.C.
																																																					rmcp_gcac[0] = (byte) 0x06;
																																																					rmcp_gcac[1] = (byte) 0x00;
																																																					rmcp_gcac[2] = (byte) 0xFF;
																																																					rmcp_gcac[3] = (byte) 0x07;

																																																					rmcp_gcac[4] = (byte) 0x00;
																																																					rmcp_gcac[5] = (byte) 0x00;
																																																					rmcp_gcac[6] = (byte) 0x00;
																																																					rmcp_gcac[7] = (byte) 0x00;
																																																					rmcp_gcac[8] = (byte) 0x00;
																																																					rmcp_gcac[9] = (byte) 0x00;
																																																					rmcp_gcac[10] = (byte) 0x00;
																																																					rmcp_gcac[11] = (byte) 0x00;
																																																					rmcp_gcac[12] = (byte) 0x00;

																																																					rmcp_gcac[13] = (byte) 0x09;
																																																					rmcp_gcac[14] = (byte) 0x20;
																																																					rmcp_gcac[15] = (byte) 0x18;

																																																					rmcp_gcac[16] = (byte) 0xC8;
																																																					rmcp_gcac[17] = (byte) 0x81;
																																																					rmcp_gcac[18] = (byte) 0x00;
																																																					rmcp_gcac[19] = (byte) 0x38;

																																																					rmcp_gcac[20] = (byte) 0x8E;
																																																					rmcp_gcac[21] = (byte) 0x04;

																																																					byte zzz_1 = ((byte) ((byte) rmcp_gcac[17]
																																																							+ (byte) rmcp_gcac[18]
																																																							+ (byte) rmcp_gcac[19]
																																																							+ (byte) rmcp_gcac[20] + (byte) rmcp_gcac[21]));
																																																					byte zzzz_1 = (byte) ~zzz_1;
																																																					zzzz_1++;
																																																					rmcp_gcac[22] = (byte) zzzz_1;

																																																					Packet_gcac = new DatagramPacket(
																																																							rmcp_gcac,
																																																							rmcp_gcac.length,
																																																							address,
																																																							solPort);

																																																					socket_gcac
																																																							.send(Packet_gcac);

																																																					// 接收
																																																					byte[] inbuffer_gcac = new byte[256];
																																																					Packet_gcac = new DatagramPacket(
																																																							inbuffer_gcac,
																																																							inbuffer_gcac.length);
																																																					socket_gcac
																																																							.receive(Packet_gcac);
																																																					socket_gcac
																																																							.close();

																																																					if (inbuffer_gcac != null) {
																																																						DatagramPacket Packet_osr;

																																																						int osr = 1;
																																																						try {

																																																							DatagramSocket socket_osr = new DatagramSocket();

																																																							if (osr <= 1) {
																																																								osr++;

																																																								byte[] rmcp_osr = new byte[48];

																																																								// Op
																																																								// Session
																																																								// Req
																																																								rmcp_osr[0] = (byte) 0x06;
																																																								rmcp_osr[1] = (byte) 0x00;
																																																								rmcp_osr[2] = (byte) 0xFF;
																																																								rmcp_osr[3] = (byte) 0x07;

																																																								rmcp_osr[4] = (byte) 0x06;
																																																								rmcp_osr[5] = (byte) 0x10;

																																																								rmcp_osr[6] = (byte) 0x00;
																																																								rmcp_osr[7] = (byte) 0x00;
																																																								rmcp_osr[8] = (byte) 0x00;
																																																								rmcp_osr[9] = (byte) 0x00;

																																																								rmcp_osr[10] = (byte) 0x00;
																																																								rmcp_osr[11] = (byte) 0x00;
																																																								rmcp_osr[12] = (byte) 0x00;
																																																								rmcp_osr[13] = (byte) 0x00;

																																																								rmcp_osr[14] = (byte) 0x20;
																																																								rmcp_osr[15] = (byte) 0x00;

																																																								rmcp_osr[16] = (byte) 0x07;
																																																								rmcp_osr[17] = (byte) 0x04;
																																																								rmcp_osr[18] = (byte) 0x00;
																																																								rmcp_osr[19] = (byte) 0x00;

																																																								// Remote
																																																								// Console
																																																								// Session
																																																								// ID
																																																								rmcp_osr[20] = (byte) 0xa4;
																																																								rmcp_osr[21] = (byte) 0xa3;

																																																								rmcp_osr[22] = (byte) 0xa2;
																																																								rmcp_osr[23] = (byte) 0xa1;

																																																								rmcp_osr[24] = (byte) 0x00;
																																																								rmcp_osr[25] = (byte) 0x00;
																																																								rmcp_osr[26] = (byte) 0x00;
																																																								rmcp_osr[27] = (byte) 0x08;
																																																								rmcp_osr[28] = (byte) 0x01;
																																																								rmcp_osr[29] = (byte) 0x00;
																																																								rmcp_osr[30] = (byte) 0x00;
																																																								rmcp_osr[31] = (byte) 0x00;

																																																								rmcp_osr[32] = (byte) 0x01;
																																																								rmcp_osr[33] = (byte) 0x00;
																																																								rmcp_osr[34] = (byte) 0x00;
																																																								rmcp_osr[35] = (byte) 0x08;
																																																								rmcp_osr[36] = (byte) 0x00;
																																																								rmcp_osr[37] = (byte) 0x00;
																																																								rmcp_osr[38] = (byte) 0x00;
																																																								rmcp_osr[39] = (byte) 0x00;

																																																								rmcp_osr[40] = (byte) 0x02;
																																																								rmcp_osr[41] = (byte) 0x00;
																																																								rmcp_osr[42] = (byte) 0x00;
																																																								rmcp_osr[43] = (byte) 0x08;
																																																								rmcp_osr[44] = (byte) 0x00;
																																																								rmcp_osr[45] = (byte) 0x00;
																																																								rmcp_osr[46] = (byte) 0x00;
																																																								rmcp_osr[47] = (byte) 0x00;

																																																								Packet_osr = new DatagramPacket(
																																																										rmcp_osr,
																																																										rmcp_osr.length,
																																																										address,
																																																										solPort);

																																																								socket_osr
																																																										.send(Packet_osr);

																																																								// 接收

																																																								Packet_osr = new DatagramPacket(
																																																										inbuffer_osr,
																																																										inbuffer_osr.length);
																																																								socket_osr
																																																										.receive(Packet_osr);
																																																								socket_osr
																																																										.close();

																																																								if (inbuffer_osr != null) {

																																																									DatagramPacket Packet_rakp1;

																																																									int rakp1 = 1;
																																																									try {

																																																										DatagramSocket socket_rakp1 = new DatagramSocket();

																																																										if (rakp1 <= 1) {
																																																											rakp1++;

																																																											byte[] rmcp_rakp1 = new byte[60];

																																																											// RAKP
																																																											// Msg
																																																											// 1
																																																											rmcp_rakp1[0] = (byte) 0x06; // IMPI
																																																																			// header
																																																											rmcp_rakp1[1] = (byte) 0x00;
																																																											rmcp_rakp1[2] = (byte) 0xFF;
																																																											rmcp_rakp1[3] = (byte) 0x07;
																																																											rmcp_rakp1[4] = (byte) 0x06; // AT/F
																																																																			// 6h
																																																																			// format
																																																																			// =
																																																																			// RMCP+
																																																											rmcp_rakp1[5] = (byte) 0x12; // Payload
																																																																			// Type
																																																																			// 1101
																																																																			// 0000
																																																											rmcp_rakp1[6] = (byte) 0x00; // IMPI
																																																																			// RMCP+
																																																																			// Session
																																																																			// ID
																																																											rmcp_rakp1[7] = (byte) 0x00;
																																																											rmcp_rakp1[8] = (byte) 0x00;
																																																											rmcp_rakp1[9] = (byte) 0x00;
																																																											rmcp_rakp1[10] = (byte) 0x00; // Session
																																																																			// Seq.
																																																																			// No.
																																																											rmcp_rakp1[11] = (byte) 0x00;
																																																											rmcp_rakp1[12] = (byte) 0x00;
																																																											rmcp_rakp1[13] = (byte) 0x00;
																																																											rmcp_rakp1[14] = (byte) 0x2C; // IMPI
																																																																			// payload
																																																																			// length
																																																											rmcp_rakp1[15] = (byte) 0x00;
																																																											// Msg
																																																											// Tag
																																																											rmcp_rakp1[16] = (byte) 0x08;
																																																											rmcp_rakp1[17] = (byte) 0x00; // byte
																																																																			// 2:4
																																																																			// -
																																																																			// reserved
																																																											rmcp_rakp1[18] = (byte) 0x00;
																																																											rmcp_rakp1[19] = (byte) 0x00;

																																																											rmcp_rakp1[20] = (byte) inbuffer_osr[24];
																																																											rmcp_rakp1[21] = (byte) inbuffer_osr[25];
																																																											rmcp_rakp1[22] = (byte) inbuffer_osr[26];
																																																											rmcp_rakp1[23] = (byte) inbuffer_osr[27];

																																																											// Remote
																																																											// Console
																																																											// Random
																																																											// Number,自己產生
																																																											rmcp_rakp1[24] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[25] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[26] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[27] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[28] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[29] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[30] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[31] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[32] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[33] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[34] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[35] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[36] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[37] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[38] = (byte) randomG_1
																																																													.nextInt(128);
																																																											rmcp_rakp1[39] = (byte) randomG_1
																																																													.nextInt(128);

																																																											rmcp_rakp1[40] = (byte) userPriv; // level

																																																											rmcp_rakp1[41] = (byte) 0x00; // byte
																																																																			// 2:4
																																																																			// -
																																																																			// reserved
																																																											rmcp_rakp1[42] = (byte) 0x00;

																																																											rmcp_rakp1[43] = (byte) ID_length();// User
																																																																				// Name
																																																																				// Length
																																																																				// System.out.println("============");
																																																											// System.out.println(Integer.toHexString(rmcp_rakp1[43]));
																																																											rmcp_rakp1[44] = (byte) 0x00;
																																																											rmcp_rakp1[45] = (byte) 0x00;
																																																											rmcp_rakp1[46] = (byte) 0x00;
																																																											rmcp_rakp1[47] = (byte) 0x00;
																																																											rmcp_rakp1[48] = (byte) 0x00;
																																																											rmcp_rakp1[49] = (byte) 0x00;
																																																											rmcp_rakp1[50] = (byte) 0x00;
																																																											rmcp_rakp1[51] = (byte) 0x00;
																																																											rmcp_rakp1[52] = (byte) 0x00;
																																																											rmcp_rakp1[53] = (byte) 0x00;
																																																											rmcp_rakp1[54] = (byte) 0x00;
																																																											rmcp_rakp1[55] = (byte) 0x00;
																																																											rmcp_rakp1[56] = (byte) 0x00;
																																																											rmcp_rakp1[57] = (byte) 0x00;
																																																											rmcp_rakp1[58] = (byte) 0x00;
																																																											rmcp_rakp1[59] = (byte) 0x00;

																																																											int id_2 = 0;
																																																											for (id_2 = 0; id_2 < ID_length_int(); id_2++) {
																																																												rmcp_rakp1[44 + id_2] = (byte) id_byte[id_2];
																																																												// System.out.println(Integer.toHexString(rmcp_rakp1[44
																																																												// +
																																																												// id_2]));
																																																											}
																																																											// System.out.println("============");

																																																											Packet_rakp1 = new DatagramPacket(
																																																													rmcp_rakp1,
																																																													rmcp_rakp1.length,
																																																													address,
																																																													solPort);

																																																											socket_rakp1
																																																													.send(Packet_rakp1);

																																																											// 接收
																																																											byte[] inbuffer_rakp1 = new byte[256];
																																																											Packet_rakp1 = new DatagramPacket(
																																																													inbuffer_rakp1,
																																																													inbuffer_rakp1.length);
																																																											socket_rakp1
																																																													.receive(Packet_rakp1);
																																																											socket_rakp1
																																																													.close();
																																																											if (inbuffer_rakp1 != null)

																																																											{

																																																												DatagramPacket Packet_rakp3;

																																																												int rakp3 = 1;
																																																												try {

																																																													DatagramSocket socket_rakp3 = new DatagramSocket();

																																																													if (rakp3 <= 1) {
																																																														rakp3++;

																																																														// Data
																																																														// and
																																																														// Keys
																																																														// to
																																																														// Auth
																																																														// code
																																																														byte[] Data3 = new byte[40];
																																																														byte[] Key3 = new byte[25];
																																																														Data3[0] = (byte) inbuffer_rakp1[24];
																																																														Data3[1] = (byte) inbuffer_rakp1[25];
																																																														Data3[2] = (byte) inbuffer_rakp1[26];
																																																														Data3[3] = (byte) inbuffer_rakp1[27];
																																																														Data3[4] = (byte) inbuffer_rakp1[28];
																																																														Data3[5] = (byte) inbuffer_rakp1[29];
																																																														Data3[6] = (byte) inbuffer_rakp1[30];
																																																														Data3[7] = (byte) inbuffer_rakp1[31];
																																																														Data3[8] = (byte) inbuffer_rakp1[32];
																																																														Data3[9] = (byte) inbuffer_rakp1[33];
																																																														Data3[10] = (byte) inbuffer_rakp1[34];
																																																														Data3[11] = (byte) inbuffer_rakp1[35];
																																																														Data3[12] = (byte) inbuffer_rakp1[36];
																																																														Data3[13] = (byte) inbuffer_rakp1[37];
																																																														Data3[14] = (byte) inbuffer_rakp1[38];
																																																														Data3[15] = (byte) inbuffer_rakp1[39];
																																																														Data3[16] = (byte) inbuffer_rakp1[20];
																																																														Data3[17] = (byte) inbuffer_rakp1[21];
																																																														Data3[18] = (byte) inbuffer_rakp1[22];
																																																														Data3[19] = (byte) inbuffer_rakp1[23];
																																																														Data3[20] = (byte) userPriv;
																																																														Data3[21] = (byte) ID_length();

																																																														byte id_byte3[] = siteField_ID
																																																																.getText()
																																																																.getBytes();
																																																														byte pw_byte3[] = siteField_PW
																																																																.getText()
																																																																.getBytes();

																																																														Data3[22] = (byte) 0x00;
																																																														Data3[23] = (byte) 0x00;
																																																														Data3[24] = (byte) 0x00;
																																																														Data3[25] = (byte) 0x00;
																																																														Data3[26] = (byte) 0x00;
																																																														Data3[27] = (byte) 0x00;
																																																														Data3[28] = (byte) 0x00;
																																																														Data3[29] = (byte) 0x00;
																																																														Data3[30] = (byte) 0x00;
																																																														Data3[31] = (byte) 0x00;
																																																														Data3[32] = (byte) 0x00;
																																																														Data3[33] = (byte) 0x00;
																																																														Data3[34] = (byte) 0x00;
																																																														Data3[35] = (byte) 0x00;
																																																														Data3[36] = (byte) 0x00;
																																																														Data3[37] = (byte) 0x00;
																																																														Data3[38] = (byte) 0x00;
																																																														Data3[39] = (byte) 0x00;

																																																														int pw3 = 0;
																																																														for (pw3 = 0; pw3 < PW_length_int(); pw3++) {
																																																															Key3[pw3] = (byte) pw_byte3[pw3];
																																																														}

																																																														for (pw3 = 0; pw3 < ID_length_int(); pw3++) {
																																																															Data3[22 + pw3] = (byte) id_byte3[pw3];
																																																														}
																																																														int Key_Length3 = PW_length_int();
																																																														int ID_Length3 = ID_length_int();
																																																														int Data_Length3 = 22 + ID_Length3;

																																																														byte[] Authcode3 = HMACSHA1(
																																																																Data3,
																																																																Data_Length3,
																																																																Key3,
																																																																Key_Length3);

																																																														byte[] rmcp_rakp3 = new byte[44];

																																																														// RAKP
																																																														// Msg
																																																														// 3
																																																														rmcp_rakp3[0] = (byte) 0x06; // IMPI
																																																																						// header
																																																														rmcp_rakp3[1] = (byte) 0x00;
																																																														rmcp_rakp3[2] = (byte) 0xFF;
																																																														rmcp_rakp3[3] = (byte) 0x07;
																																																														rmcp_rakp3[4] = (byte) 0x06; // AT/F
																																																																						// 6h
																																																																						// format
																																																																						// =
																																																																						// RMCP+
																																																														rmcp_rakp3[5] = (byte) 0x14; // Payload
																																																																						// Type
																																																																						// 1101
																																																																						// 0000
																																																														rmcp_rakp3[6] = (byte) 0x00; // IMPI
																																																																						// RMCP+
																																																																						// Session
																																																																						// ID
																																																														rmcp_rakp3[7] = (byte) 0x00;
																																																														rmcp_rakp3[8] = (byte) 0x00;
																																																														rmcp_rakp3[9] = (byte) 0x00;
																																																														rmcp_rakp3[10] = (byte) 0x00; // Session
																																																														// Seq.
																																																														// No.
																																																														rmcp_rakp3[11] = (byte) 0x00;
																																																														rmcp_rakp3[12] = (byte) 0x00;
																																																														rmcp_rakp3[13] = (byte) 0x00;
																																																														rmcp_rakp3[14] = (byte) 0x1c; // IMPI
																																																																						// payload
																																																																						// length
																																																														rmcp_rakp3[15] = (byte) 0x00;
																																																														// Msg
																																																														// Tag
																																																														rmcp_rakp3[16] = (byte) 0x09;
																																																														rmcp_rakp3[17] = (byte) 0x00; // byte
																																																																						// 2:4
																																																																						// -
																																																																						// reserved
																																																														rmcp_rakp3[18] = (byte) 0x00;
																																																														rmcp_rakp3[19] = (byte) 0x00;

																																																														rmcp_rakp3[20] = (byte) inbuffer_osr[24];
																																																														rmcp_rakp3[21] = (byte) inbuffer_osr[25];
																																																														rmcp_rakp3[22] = (byte) inbuffer_osr[26];
																																																														rmcp_rakp3[23] = (byte) inbuffer_osr[27];

																																																														rmcp_rakp3[24] = (byte) Authcode3[0];
																																																														rmcp_rakp3[25] = (byte) Authcode3[1];
																																																														rmcp_rakp3[26] = (byte) Authcode3[2];
																																																														rmcp_rakp3[27] = (byte) Authcode3[3];
																																																														rmcp_rakp3[28] = (byte) Authcode3[4];
																																																														rmcp_rakp3[29] = (byte) Authcode3[5];
																																																														rmcp_rakp3[30] = (byte) Authcode3[6];
																																																														rmcp_rakp3[31] = (byte) Authcode3[7];
																																																														rmcp_rakp3[32] = (byte) Authcode3[8];
																																																														rmcp_rakp3[33] = (byte) Authcode3[9];
																																																														rmcp_rakp3[34] = (byte) Authcode3[10];
																																																														rmcp_rakp3[35] = (byte) Authcode3[11];
																																																														rmcp_rakp3[36] = (byte) Authcode3[12];
																																																														rmcp_rakp3[37] = (byte) Authcode3[13];
																																																														rmcp_rakp3[38] = (byte) Authcode3[14];
																																																														rmcp_rakp3[39] = (byte) Authcode3[15];
																																																														rmcp_rakp3[40] = (byte) Authcode3[16];
																																																														rmcp_rakp3[41] = (byte) Authcode3[17];
																																																														rmcp_rakp3[42] = (byte) Authcode3[18];
																																																														rmcp_rakp3[43] = (byte) Authcode3[19];

																																																														Packet_rakp3 = new DatagramPacket(
																																																																rmcp_rakp3,
																																																																rmcp_rakp3.length,
																																																																address,
																																																																solPort);

																																																														socket_rakp3
																																																																.send(Packet_rakp3);

																																																														// 接收
																																																														byte[] inbuffer_rakp3 = new byte[256];
																																																														Packet_rakp3 = new DatagramPacket(
																																																																inbuffer_rakp3,
																																																																inbuffer_rakp3.length);
																																																														socket_rakp3
																																																																.receive(Packet_rakp3);
																																																														socket_rakp3
																																																																.close();

																																																														if (inbuffer_rakp3 != null) {
																																																															DatagramPacket Packet_ipmi_1;
																																																															DatagramPacket rPacket_ipmi_1;
																																																															int ipmi_1 = 1;
																																																															try {

																																																																DatagramSocket socket_ipmi_1 = new DatagramSocket();
																																																																if (ipmi_1 <= 1) {
																																																																	ipmi_1++;

																																																																	// ipmi
																																																																	// MSG
																																																																	ipmi_msg_1[0] = (byte) 0x06;
																																																																	ipmi_msg_1[1] = (byte) 0x00;
																																																																	ipmi_msg_1[2] = (byte) 0xff;
																																																																	ipmi_msg_1[3] = (byte) 0x07;

																																																																	ipmi_msg_1[4] = (byte) 0x06;

																																																																	ipmi_msg_1[5] = (byte) 0x00;

																																																																	ipmi_msg_1[6] = (byte) inbuffer_osr[24];
																																																																	ipmi_msg_1[7] = (byte) inbuffer_osr[25];
																																																																	ipmi_msg_1[8] = (byte) inbuffer_osr[26];
																																																																	ipmi_msg_1[9] = (byte) inbuffer_osr[27];

																																																																	ipmi_msg_1[10] = (byte) 0x03;// all
																																																																									// 4
																																																																									// bytes
																																																																									// accumulate
																																																																									// 01
																																																																	ipmi_msg_1[11] = (byte) 0x00;
																																																																	ipmi_msg_1[12] = (byte) 0x00;
																																																																	ipmi_msg_1[13] = (byte) 0x00;

																																																																	ipmi_msg_1[14] = (byte) 0x08;// message
																																																																									// length
																																																																	ipmi_msg_1[15] = (byte) 0x00;

																																																																	ipmi_msg_1[16] = (byte) 0x20;
																																																																	ipmi_msg_1[17] = (byte) 0x18;
																																																																	ipmi_msg_1[18] = (byte) 0xc8;// checksum
																																																																									// 2's
																																																																									// complement

																																																																	ipmi_msg_1[19] = (byte) 0x81;// 1
																																																																									// byte
																																																																									// accumulate
																																																																									// 04
																																																																									// (100:bit)
																																																																	ipmi_msg_1[20]++;
																																																																	ipmi_msg_1[20]++;
																																																																	ipmi_msg_1[20]++;
																																																																	ipmi_msg_1[20]++;

																																																																	ipmi_msg_1[21] = (byte) 0x3b;
																																																																	ipmi_msg_1[22] = (byte) 0x04;
																																																																	byte bbb_2 = ((byte) ((byte) ipmi_msg_1[19]
																																																																			+ (byte) ipmi_msg_1[20]
																																																																			+ (byte) ipmi_msg_1[21] + (byte) ipmi_msg_1[22]));
																																																																	byte bbbb_2 = (byte) ~bbb_2;
																																																																	bbbb_2++;
																																																																	ipmi_msg_1[23] = (byte) bbbb_2;

																																																																	Packet_ipmi_1 = new DatagramPacket(
																																																																			ipmi_msg_1,
																																																																			ipmi_msg_1.length,
																																																																			address,
																																																																			solPort);
																																																																	socket_ipmi_1
																																																																			.send(Packet_ipmi_1);

																																																																	// 接收
																																																																	byte[] inbuffer_ipmi_1 = new byte[256];
																																																																	rPacket_ipmi_1 = new DatagramPacket(
																																																																			inbuffer_ipmi_1,
																																																																			inbuffer_ipmi_1.length);
																																																																	socket_ipmi_1
																																																																			.receive(rPacket_ipmi_1);
																																																																	socket_ipmi_1
																																																																			.close();

																																																																	if (inbuffer_ipmi_1 != null) {
																																																																		DatagramPacket Packet_ipmi_2;
																																																																		DatagramPacket rPacket_ipmi_2;
																																																																		int ipmi_2 = 1;
																																																																		try {
																																																																			byte[] ipmi_msg_2 = new byte[29];

																																																																			DatagramSocket socket_ipmi_2 = new DatagramSocket();
																																																																			local_port = socket_ipmi_2
																																																																					.getLocalPort();
																																																																			if (ipmi_2 <= 1) {
																																																																				ipmi_2++;
																																																																				ipmi_msg_2[0] = (byte) 0x06;
																																																																				ipmi_msg_2[1] = (byte) 0x00;
																																																																				ipmi_msg_2[2] = (byte) 0xff;
																																																																				ipmi_msg_2[3] = (byte) 0x07;

																																																																				ipmi_msg_2[4] = (byte) 0x06;

																																																																				ipmi_msg_2[5] = (byte) 0x00;

																																																																				ipmi_msg_2[6] = (byte) inbuffer_osr[24];
																																																																				ipmi_msg_2[7] = (byte) inbuffer_osr[25];
																																																																				ipmi_msg_2[8] = (byte) inbuffer_osr[26];
																																																																				ipmi_msg_2[9] = (byte) inbuffer_osr[27];

																																																																				ipmi_msg_2[10] = ++ipmi_msg_1[10];// 同步
																																																																				ipmi_msg_2[11] = (byte) 0x00;
																																																																				ipmi_msg_2[12] = (byte) 0x00;
																																																																				ipmi_msg_2[13] = (byte) 0x00;

																																																																				ipmi_msg_2[14] = (byte) 0x0D;// message
																																																																												// length
																																																																				ipmi_msg_2[15] = (byte) 0x00;

																																																																				ipmi_msg_2[16] = (byte) 0x20;
																																																																				ipmi_msg_2[17] = (byte) 0x18;
																																																																				ipmi_msg_2[18] = (byte) 0xc8;// checksum
																																																																												// 2's
																																																																												// complement

																																																																				ipmi_msg_2[19] = (byte) 0x81;// 1
																																																																												// byte
																																																																												// accumulate
																																																																												// 04
																																																																												// (100:bit)
																																																																				ipmi_msg_1[20]++;
																																																																				ipmi_msg_1[20]++;
																																																																				ipmi_msg_1[20]++;
																																																																				ipmi_msg_1[20]++;
																																																																				ipmi_msg_2[20] = ipmi_msg_1[20];// 同1
																																																																				ipmi_msg_2[21] = (byte) 0x48;
																																																																				ipmi_msg_2[22] = (byte) 0x01;
																																																																				ipmi_msg_2[23] = (byte) 0x01;
																																																																				ipmi_msg_2[24] = (byte) 0xc6;
																																																																				ipmi_msg_2[25] = (byte) 0x00;
																																																																				ipmi_msg_2[26] = (byte) 0x00;
																																																																				ipmi_msg_2[27] = (byte) 0x00;

																																																																				byte ccc_2 = ((byte) ((byte) ipmi_msg_2[19]
																																																																						+ (byte) ipmi_msg_2[20]
																																																																						+ (byte) ipmi_msg_2[21]
																																																																						+ (byte) ipmi_msg_2[22]
																																																																						+ (byte) ipmi_msg_2[23]
																																																																						+ (byte) ipmi_msg_2[24]
																																																																						+ (byte) ipmi_msg_2[25]
																																																																						+ (byte) ipmi_msg_2[26] + (byte) ipmi_msg_2[27]));
																																																																				byte cccc_2 = (byte) ~ccc_2;
																																																																				cccc_2++;
																																																																				ipmi_msg_2[28] = (byte) cccc_2;

																																																																				Packet_ipmi_2 = new DatagramPacket(
																																																																						ipmi_msg_2,
																																																																						ipmi_msg_2.length,
																																																																						address,
																																																																						solPort);
																																																																				socket_ipmi_2
																																																																						.send(Packet_ipmi_2);
																																																																				byte[] inbuffer_ipmi_2 = new byte[256];
																																																																				rPacket_ipmi_2 = new DatagramPacket(
																																																																						inbuffer_ipmi_2,
																																																																						inbuffer_ipmi_2.length);
																																																																				socket_ipmi_2
																																																																						.receive(rPacket_ipmi_2);

																																																																				socket_ipmi_2
																																																																						.close();
																																																																				connect(siteField
																																																																						.getText());

																																																																			}
																																																																		} catch (IOException e) {
																																																																		}
																																																																	}// end
																																																																		// if

																																																																}
																																																															} catch (IOException e) {
																																																															}
																																																														}// end
																																																															// if

																																																													}
																																																												} catch (IOException e) {
																																																												}
																																																											}// end
																																																												// if

																																																										}
																																																									} catch (IOException e) {
																																																									}
																																																								}// end
																																																									// if

																																																							}
																																																						} catch (IOException e) {
																																																						}
																																																					}// end
																																																						// if

																																																				}
																																																			} catch (IOException e) {
																																																			}
																																																		}// end
																																																			// if

																																																	}
																																																} catch (IOException e) {
																																																}
																																															}// end
																																																// if

																																														}
																																													} catch (IOException e) {
																																													}
																																												}// end
																																													// if

																																											}
																																										} catch (IOException e) {
																																										}
																																									}// end
																																										// if

																																								}
																																							} catch (IOException e) {
																																							}
																																						}// end
																																							// if

																																					}
																																				} catch (IOException e) {
																																				}
																																			}// end
																																				// if

																																		}
																																	} catch (IOException e) {
																																	}
																																}// end
																																	// if

																															}
																														} catch (IOException e) {
																														}
																													}// end
																														// if

																												}
																											} catch (IOException e) {
																											}
																										}// end
																											// if

																									}
																								} catch (IOException e) {
																								}
																							}// end
																								// if

																						}
																					} catch (IOException e) {
																					}
																				}// end
																					// if

																			}
																		} catch (IOException e) {
																		}
																	}// end if

																}
															} catch (IOException e) {
															}
														}// end if

													}
												} catch (IOException e) {
												}
											}// end if

										}
									} catch (IOException e) {
									}
								}// end if

							}
						} catch (IOException e) {
						}
					}// end if

				}
			} catch (IOException e) {
			}
		}// end if
	}

}