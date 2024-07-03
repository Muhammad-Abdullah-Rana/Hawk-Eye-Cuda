import { Injectable } from '@angular/core';
import { Observable, Observer } from 'rxjs';

const WEBSOCKET_URL = 'ws://localhost:8080/image_frame';

@Injectable({
  providedIn: 'root'
})
export class ImageStreamService {

  private webSocket: WebSocket | null = null;

  getImageStream(): Observable<ArrayBuffer> {
    return new Observable((observer: Observer<ArrayBuffer>) => {
      if (!this.webSocket) {
        this.webSocket = new WebSocket(WEBSOCKET_URL);
        this.webSocket.onmessage = (event) => observer.next(event.data);
        this.webSocket.onerror = (error) => observer.error(error);
      }
      return () => {
        if (this.webSocket) {
          this.webSocket.close();
          this.webSocket = null;
        }
      };
    });
  }
}
