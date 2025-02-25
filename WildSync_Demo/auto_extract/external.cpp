bool VorbisDecoder::seek(double s)
{
	int result = 0;

	if (s >= 0.000001) {
		result = ov_time_seek(&handle, s);
	} else {
		// ...
	}

	// ...
}

