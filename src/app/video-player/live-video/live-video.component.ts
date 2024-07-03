import { Component, OnInit, OnDestroy } from '@angular/core';
import { Subscription, map } from 'rxjs';
import { ImageStreamService } from '../../shared/services/image-stream/image-stream.service';

@Component({
  selector: 'app-image-stream',
  templateUrl: './live-video.component.html',
  styleUrls: ['./live-video.component.scss']
})

export class LiveVideoComponent implements OnInit, OnDestroy {
  protected videos: string[] = [];
  private subscription: Subscription;
  private videoNum: number = 0;
  constructor(private imageStreamService: ImageStreamService) {
    for(let i = 0; i < 3; i++){
      this.videos.push("/assets/init_pic.jpg");
    }
   }

  ngOnInit() {
    this.subscription = this.imageStreamService.getImageStream()
      .subscribe(imageData => {
        const blob = new Blob([imageData], { type: 'image/jpeg' }); // Assuming JPEG format
        const url = URL.createObjectURL(blob);
        this.displayImage(url);
      });
  }

  ngOnDestroy() {
    this.subscription.unsubscribe();
  }

  displayImage(url: string) {
    this.videoNum = (this.videoNum + 1) % this.videos.length;
    this.videos[this.videoNum] = url;
  }

}