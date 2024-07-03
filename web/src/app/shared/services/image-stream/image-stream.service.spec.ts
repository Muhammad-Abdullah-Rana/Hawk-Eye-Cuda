import { TestBed } from '@angular/core/testing';

import { ImageStreamService } from './image-stream.service';

describe('ImageStreamService', () => {
  let service: ImageStreamService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ImageStreamService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
