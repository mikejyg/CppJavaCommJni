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
	private native void javaToCpp(String str);

	private Thread listenerThread;
	private boolean active = false;
	private static Listener listener;
	
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
			// use this to speed up the calls.
//    		String str="";
//    		for (int i=0; i<1000; i++) {
//    			str = str + String.valueOf(random.nextInt()) + " ";
//    		}
	    	while (active) {
	    		String str="";
	    		for (int i=0; i<1000; i++) {
	    			str = str + String.valueOf(random.nextInt()) + " ";
	    		}
	    		getInstance().javaToCpp(str);
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
	    		getInstance().javaToCpp(String.valueOf(random.nextInt()));
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
	
}
