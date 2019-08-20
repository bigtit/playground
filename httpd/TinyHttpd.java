package httpd;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.nio.channels.FileChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.time.Instant;
import java.util.Date;
import java.util.Iterator;
import java.util.Objects;
import java.util.Optional;
import java.util.Set;

public class TinyHttpd {
  private static final int DEFAULT_PORT = 8080;
  private static final int DEFAULT_BUFFER_SIZE = 4096;
  private static final String CRLF2 = "\r\n\r\n";

  private int port = 0;
  private Selector selector;
  private ServerSocketChannel ssc;
  private ServerSocketChannel ssc2;

  public TinyHttpd() {
    this(DEFAULT_PORT);
  }
  public TinyHttpd(int p) {
    this.port = p;
  }
  public void start() throws IOException {
    ssc = ServerSocketChannel.open();
    ssc.socket().bind(new InetSocketAddress("localhost", port));
    ssc.configureBlocking(false);

    ssc2 = ServerSocketChannel.open();
    ssc2.socket().bind(new InetSocketAddress("localhost", port+1));
    ssc2.configureBlocking(false);

    selector = Selector.open();
    ssc.register(selector, SelectionKey.OP_ACCEPT);
    ssc2.register(selector, SelectionKey.OP_ACCEPT);

    System.out.println(String.format("TinyHttpd is starting, listening port %d...", port));

    while (true) {
      int n = selector.select();
      if (n == 0) continue;

      Set<SelectionKey> sks = selector.selectedKeys();
      Iterator<SelectionKey> it = sks.iterator();
      while (it.hasNext()) {
        SelectionKey sk = it.next();
        it.remove();
        if (sk.isAcceptable()) {
          connect(sk);
        } else if (sk.isReadable()) {
          request(sk);
        } else if (sk.isWritable()) {
          response(sk);
        }
      }
    }
  }
  private void connect(SelectionKey sk) throws IOException {
    ServerSocketChannel s = (ServerSocketChannel) sk.channel();
    SocketChannel c = s.accept();
    c.configureBlocking(false);
    c.register(selector, SelectionKey.OP_READ);
    if (s == ssc) System.out.println("ssc accept");
    else System.out.println("ssc2 accept");
  }
  private void request(SelectionKey sk) throws IOException {
    SocketChannel ch = (SocketChannel) sk.channel();
    ByteBuffer buf = ByteBuffer.allocate(DEFAULT_BUFFER_SIZE);
    ch.read(buf);
    buf.flip();
    String msg = StandardCharsets.UTF_8.decode(buf).toString();
    // process header and body
    // System.out.println(msg.substring(msg.indexOf(CRLF2)+4));
    String[] msgList = msg.split(CRLF2);
    if (msgList.length > 0) {
      System.out.println("--------------header--------------");
      System.out.println(msgList[0]);
      if (msgList.length > 1) {
        System.out.println("--------------body--------------");
        System.out.println(msgList[1]);
      }
    }
    sk.interestOps(SelectionKey.OP_WRITE);
  }
  private void response(SelectionKey sk) throws IOException {
    SocketChannel ch = (SocketChannel) sk.channel();
    String msg = "success";
    ch.write(ByteBuffer.wrap(msg.getBytes(StandardCharsets.UTF_8)));

    ch.close();
  }

  public static void main(String[] args) throws IOException {
    new TinyHttpd().start();
  }
}
