import java.util.concurrent.Semaphore;

public class TrafficController {

    private Semaphore semaphore;

    public TrafficController () {
        semaphore = new Semaphore(1);
    }

    private static void nap(int ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException e) {}
    }

    private void onBridge() {
        try {
            Thread wait = new Thread(new Runnable() {
                public void run() {
                    while (!semaphore.tryAcquire()) {
                        nap(25);
                    }
                }
            });
            wait.start();
            wait.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void enterLeft() {
        System.out.println("chegou esquerda");
        onBridge();
    }

    public void enterRight() {
        System.out.println("chegou direita");
        onBridge();
    }

    public void leaveLeft() {
        System.out.println("saiu esquerda");
        semaphore.release();
    }

    public void leaveRight() {
        System.out.println("saiu direita");
        semaphore.release();
    }
}