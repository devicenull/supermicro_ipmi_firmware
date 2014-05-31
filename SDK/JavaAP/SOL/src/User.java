import java.awt.Cursor;
import java.awt.event.*;

public class User implements KeyListener, MouseListener, MouseMotionListener {
	private Session parent;
	private Resource resource;
	private int pressX, pressY, dragX, dragY, releaseX, releaseY;

	public User(Session p, Resource r) {
		parent = p;
		resource = r;
	}

	public static byte[] buf = new byte[6];
	public static int len;

	public void keyPressed(KeyEvent e) {
		int len;
		switch (e.getKeyCode()) {
		case KeyEvent.VK_UP:
			if (parent.getKeypadMode() == VT100_PLUS.NUMERIC_KEYPAD) {
				buf[0] = 0x1b;
				buf[1] = 0x5b;
				buf[2] = 'A';
				len = 3;
			} else {
				buf[0] = 0x1b;
				buf[1] = 0x4f;
				buf[2] = 'A';
				len = 3;
			}
			break;
		case KeyEvent.VK_DOWN:
			if (parent.getKeypadMode() == VT100_PLUS.NUMERIC_KEYPAD) {
				buf[0] = 0x1b;
				buf[1] = 0x5b;
				buf[2] = 'B';
				len = 3;
			} else {
				buf[0] = 0x1b;
				buf[1] = 0x4f;
				buf[2] = 'B';
				len = 3;
			}
			break;
		case KeyEvent.VK_RIGHT:
			if (parent.getKeypadMode() == VT100_PLUS.NUMERIC_KEYPAD) {
				buf[0] = 0x1b;
				buf[1] = 0x5b;
				buf[2] = 'C';
				len = 3;
			} else {
				buf[0] = 0x1b;
				buf[1] = 0x4f;
				buf[2] = 'C';
				len = 3;
			}
			break;
		case KeyEvent.VK_LEFT:
			if (parent.getKeypadMode() == VT100_PLUS.NUMERIC_KEYPAD) {
				buf[0] = 0x1b;
				buf[1] = 0x5b;
				buf[2] = 'D';
				len = 3;
			} else {
				buf[0] = 0x1b;
				buf[1] = 0x4f;
				buf[2] = 'D';
				len = 3;
			}
			break;
		case KeyEvent.VK_INSERT:
			buf[0] = 0x1b;
			buf[1] = 0x2b;
			len = 2;
			break;
		case KeyEvent.VK_HOME:
			if (parent.getKeypadMode() == VT100_PLUS.NUMERIC_KEYPAD) {
				buf[0] = 0x1b;
				buf[1] = 0x68;
				len = 2;
			} else {
				buf[0] = 0x1b;
				buf[1] = 0x4f;
				buf[2] = 'H';
				len = 3;
			}
			break;
		case KeyEvent.VK_PAGE_UP:
			buf[0] = 0x1b;
			buf[1] = 0x3f;
			len = 2;
			break;
		case KeyEvent.VK_DELETE:
			buf[0] = 0x1b;
			buf[1] = 0x2d;
			len = 2;
			break;
		case KeyEvent.VK_END:
			if (parent.getKeypadMode() == VT100_PLUS.NUMERIC_KEYPAD) {
				buf[0] = 0x1b;
				buf[1] = 0x6b;
				len = 2;
			} else {
				buf[0] = 0x1b;
				buf[1] = 0x4f;
				buf[2] = 'F';
				len = 3;
			}
			break;
		case KeyEvent.VK_PAGE_DOWN:
			buf[0] = 0x1b;
			buf[1] = 0x2f;
			len = 2;
			break;

		case KeyEvent.VK_ALT:
			buf[0] = 0x1b;
			buf[1] = 0x5e;
			buf[2] = 0x41;
			len = 3;
			break;
		case KeyEvent.VK_CONTROL:
			buf[0] = 0x1b;
			buf[1] = 0x5e;
			buf[2] = 0x43;
			len = 3;
			break;
		case KeyEvent.VK_F1:
			buf[0] = 0x1b;
			buf[1] = 0x31;
			len = 2;
			break;
		case KeyEvent.VK_F2:
			buf[0] = 0x1b;
			buf[1] = 0x32;
			len = 2;
			break;
		case KeyEvent.VK_F3:
			buf[0] = 0x1b;
			buf[1] = 0x33;
			len = 2;
			break;
		case KeyEvent.VK_F4:
			buf[0] = 0x1b;
			buf[1] = 0x34;
			len = 2;
			break;
		case KeyEvent.VK_F5:
			buf[0] = 0x1b;
			buf[1] = 0x35;
			len = 2;
			break;
		case KeyEvent.VK_F6:
			buf[0] = 0x1b;
			buf[1] = 0x36;
			len = 2;
			break;
		case KeyEvent.VK_F7:
			buf[0] = 0x1b;
			buf[1] = 0x37;
			len = 2;
			break;
		case KeyEvent.VK_F8:
			buf[0] = 0x1b;
			buf[1] = 0x38;
			len = 2;
			break;
		case KeyEvent.VK_F9:
			buf[0] = 0x1b;
			buf[1] = 0x39;
			len = 2;
			break;
		case KeyEvent.VK_F10:
			buf[0] = 0x1b;
			buf[1] = 0x30;
			len = 2;
			break;
		case KeyEvent.VK_F11:
			buf[0] = 0x1b;
			buf[1] = 0x21;
			len = 2;
			break;
		case KeyEvent.VK_F12:
			buf[0] = 0x1b;
			buf[1] = 0x40;
			len = 2;
			break;

		default:
			len = 0;
		}

		if (len != 0) {
			parent.writeBytes(buf, 0, len);
			byte buf[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
			return;
		}

		if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
			parent.writeByte((byte) 0x1b);
			return;
		}

		if (e.getKeyCode() == KeyEvent.VK_ENTER) {
			parent.writeByte((byte) 0x0d);
			return;
		}

	}

	public void keyReleased(KeyEvent e) {
	}

	public void keyTyped(KeyEvent e) {

		if (e.isAltDown() || e.isMetaDown()) {
			return;
		}

		if (e.getKeyChar() == KeyEvent.VK_DELETE
				|| e.getKeyChar() == KeyEvent.VK_ENTER
				|| e.getKeyChar() == KeyEvent.VK_ESCAPE) {
			return;
		}

		parent.writeChar(e.getKeyChar());
	}

	public void mouseClicked(MouseEvent e) {

		do {
			if (e.getButton() == MouseEvent.BUTTON1) {
				if (parent.coverURL(e.getX(), e.getY())) {
					String url = parent.getURL(e.getX(), e.getY());

					if (url.length() != 0) {
						parent.openExternalBrowser(url);
					}
					break;
				} else if (e.getClickCount() == 2) {
					parent.selectConsequtive(e.getX(), e.getY());
					parent.repaint();
					break;
				} else if (e.getClickCount() == 3) {
					parent.selectEntireLine(e.getX(), e.getY());
					parent.repaint();
					break;
				}
			} else if (e.getButton() == MouseEvent.BUTTON2) {
				if (e.isControlDown()) {
					parent.colorPaste();
				} else {
					parent.paste();
				}
				break;
			} else if (e.getButton() == MouseEvent.BUTTON3) {
				parent.showPopup(e.getX(), e.getY());
				break;
			}

			parent.requestFocus();
			parent.resetSelected();
			parent.repaint();

		} while (false);
	}

	public void mousePressed(MouseEvent e) {
		pressX = e.getX();
		pressY = e.getY();
	}

	public void mouseReleased(MouseEvent e) {
		boolean meta, ctrl;

		if (e.getButton() != MouseEvent.BUTTON1) {
			return;
		}

		meta = e.isAltDown() || e.isMetaDown();
		ctrl = e.isControlDown();

		if (resource.getBooleanValue(Resource.COPY_ON_SELECT) == meta) {
			return;
		}

		if (ctrl) {
			parent.colorCopy();
		} else {
			parent.copy();
		}
	}

	public void mouseEntered(MouseEvent e) {
	}

	public void mouseExited(MouseEvent e) {
	}

	public void mouseMoved(MouseEvent e) {
		if (parent.coverURL(e.getX(), e.getY())) {
			parent.setCursor(new Cursor(Cursor.HAND_CURSOR));
		} else {
			parent.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
		}
	}

	public void mouseDragged(MouseEvent e) {
		dragX = e.getX();
		dragY = e.getY();

		parent.setSelected(pressX, pressY, dragX, dragY);
		parent.repaint();
	}
}
