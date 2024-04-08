package javaComm;

import java.util.Random;

public class Listener {
	static {
		System.loadLibrary("cppLib"); // Load native library hello.dll (Windows) or libhello.so (Unixes)
		//  at runtime
		// This library contains a native method called sayHello()
		System.out.println("loaded lib");
	}

	// Declare an instance native method sayHello() which receives no parameter and returns void
	private native void javaToCpp(byte[] bytes);

	private Thread listenerThread;
	private boolean active = false;
	private static Listener listener;
	
	int recvCnt=0;
	long recvByteCnt=0;
	
	private Listener() {
	}
	
	synchronized public static Listener getInstance() {
		if (listener==null) {
			System.out.println("creating Listener...");
			listener = new Listener();
		}
		return listener;
	}
	
	// for memory leak testing.
	Runnable flood=new Runnable() {
		@Override
		public void run() {
			Random random = new Random();
			
//			byte[] bytes=new byte[5000];
//			for (int i=0; i<5000; i+=4) {
//				int a=random.nextInt();
//				bytes[i]=(byte) (a & 0xff);
//				a>>=8;
//				bytes[i+1]=(byte) (a & 0xff);
//				a>>=8;
//				bytes[i+2]=(byte) (a & 0xff);
//				bytes[i+3]=(byte) (a>>8);
//			}
			
			while (active) {
				int sendLen = (int) (random.nextDouble()*5000);
				byte[] bytes=new byte[sendLen];
				
				getInstance().javaToCpp(bytes);
				
				recvCnt++;
				recvByteCnt+=sendLen;
	    	}
		}
	};
	
	Runnable periodic=new Runnable() {
		@Override
	    public void run()
	    {
			Random random = new Random();
	    	while (active) {
//	    		System.out.println("listener calling receive...");
	    		byte[] bytes=new byte[8];
	    		for (int i=0; i<8; i+=4) {
					int a=random.nextInt();
					bytes[i]=(byte) (a & 0xff);
					a>>=8;
					bytes[i+1]=(byte) (a & 0xff);
					a>>=8;
					bytes[i+2]=(byte) (a & 0xff);
					bytes[i+3]=(byte) (a>>8);
	    		}
	    		getInstance().javaToCpp(bytes);
	    		try {
					Thread.sleep(3);
				} catch (InterruptedException e) {
					active = false;
					break;
				}
	    	}
	    }
	 
	};
	
	private int start() {
		System.out.println("Listener start()...");
		
		listenerThread = new Thread(flood);
//		listenerThread = new Thread(periodic);
		
		listenerThread.setDaemon(true);
		listenerThread.start();
		active = true;
		
		System.out.println("Listener start() done.");
		
		return 0;
	}
	
	public static int startListener() {
		System.out.println("startListener()...");
		return getInstance().start();
	}
	
	public static void stopListener() throws InterruptedException {
		System.out.println("stopListener()...");
		getInstance().active=false;
		getInstance().listenerThread.join();
	}
	
	public static int isActive() {
		return getInstance().active ? 1:0;
	}
	
	public static int getRecvCnt() {
		return getInstance().recvCnt;
	}
	
	public static long getRecvByteCnt() {
		return getInstance().recvByteCnt;
	}
	
	
}
