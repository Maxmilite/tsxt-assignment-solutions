#ifndef __ONEVPL_VIDEO_PROCESS_H
#define __ONEVPL_VIDEO_PROCESS_H

#include "onevpl_util.hpp"
#include <dirent.h>
#include <string>

#define BITSTREAM_BUFFER_SIZE 4194304
#define MAJOR_API_VERSION_REQUIRED 2
#define MINOR_API_VERSION_REQUIRED 2

namespace video_process {

	const std::string out_name = "./tmp_work/out.yuv";
	FILE *source;
	FILE *sink;
	mfxLoader loader;
	mfxConfig cfg[3];
	mfxStatus sts;
	mfxVariant impl_value;
	mfxVariant cfg_val[3];
	mfxSession session;
	mfxBitstream bitstream;
	mfxVideoParam decodeParams;
	mfxFrameSurface1 *decSurfaceOut;
	mfxSyncPoint syncp;

	int run(std::string file_name) {

		source = fopen(file_name.c_str(), "rb");
		sink = fopen(out_name.c_str(), "wb");
		loader = MFXLoad();
		cfg[0] = MFXCreateConfig(loader);
		if (!source || !sink) return -1;
		if (!loader || !cfg[0]) return -1;


		impl_value.Type = MFX_VARIANT_TYPE_U32;
		impl_value.Data.U32 = MFX_IMPL_SOFTWARE;

		sts = MFXSetConfigFilterProperty(
			cfg[0], (mfxU8 *)"mfxImplDescription.Impl",
			impl_value);
		if (sts) return -1;

		cfg[1] = MFXCreateConfig(loader);
		if (!cfg[1]) return -1;
		cfg_val[1].Type = MFX_VARIANT_TYPE_U32;
		cfg_val[1].Data.U32 = MFX_CODEC_AVC;
		sts = MFXSetConfigFilterProperty(
			cfg[1],
			(mfxU8 *)"mfxImplDescription.mfxDecoderDescription.decoder.CodecID",
			cfg_val[1]);
		if (sts) return -1;

		cfg[2] = MFXCreateConfig(loader);
		if (!cfg[2]) return -1;
		cfg_val[2].Type = MFX_VARIANT_TYPE_U32;
		cfg_val[2].Data.U32 = VPLVERSION(MAJOR_API_VERSION_REQUIRED, MINOR_API_VERSION_REQUIRED);
		sts = MFXSetConfigFilterProperty(cfg[2],
										 (mfxU8 *)"mfxImplDescription.ApiVersion.Version",
										 cfg_val[2]);
		if (sts) return -1;

		sts = MFXCreateSession(loader, 0, &session);
		if (sts) return -1;

		ShowImplementationInfo(loader, 0);

		bitstream.MaxLength = BITSTREAM_BUFFER_SIZE;
		bitstream.Data = (mfxU8 *)calloc(bitstream.MaxLength, sizeof(mfxU8));
		if (!bitstream.Data) return -1;
		bitstream.CodecId = MFX_CODEC_AVC;

		sts = ReadEncodedStream(bitstream, source);
		if (sts) return -1;

		decodeParams.mfx.CodecId = MFX_CODEC_AVC;
		decodeParams.IOPattern = MFX_IOPATTERN_OUT_SYSTEM_MEMORY;
		sts = MFXVideoDECODE_DecodeHeader(session, &bitstream, &decodeParams);
		if (sts) return -1;

		sts = MFXVideoDECODE_Init(session, &decodeParams);
		if (sts) return -1;

		bool is_going = true, is_draining = false;
		int framenum = 0;

		printf("Decoding %s -> %s\n", file_name.c_str(), out_name.c_str());

		printf("Output colorspace: ");
		switch (decodeParams.mfx.FrameInfo.FourCC) {
			case MFX_FOURCC_I420: // CPU output
				printf("I420 (aka yuv420p)\n");
				break;
			case MFX_FOURCC_NV12: // GPU output
				printf("NV12\n");
				break;
			default:
				printf("Unsupported color format\n");
				return -1;
		}

		while (is_going == true) {
			// Load encoded stream if not draining
			if (is_draining == false) {
				sts = ReadEncodedStream(bitstream, source);
				if (sts != MFX_ERR_NONE)
					is_draining = true;
			}

			sts = MFXVideoDECODE_DecodeFrameAsync(session,
												  (is_draining) ? NULL : &bitstream,
												  NULL,
												  &decSurfaceOut,
												  &syncp);

			switch (sts) {
				case MFX_ERR_NONE:
					do {
						sts = decSurfaceOut->FrameInterface->Synchronize(decSurfaceOut,
																		 WAIT_100_MILLISECONDS);
						if (MFX_ERR_NONE == sts) {
							sts = WriteRawFrame_InternalMem(decSurfaceOut, sink);
							if (sts) return -1;

							framenum++;
						}
					} while (sts == MFX_WRN_IN_EXECUTION);
					break;
				case MFX_ERR_MORE_DATA:
					if (is_draining)
						is_going = false;
					break;
				case MFX_ERR_MORE_SURFACE:
					break;
				case MFX_ERR_DEVICE_LOST:
					break;
				case MFX_WRN_DEVICE_BUSY:
					break;
				case MFX_WRN_VIDEO_PARAM_CHANGED:
					break;
				case MFX_ERR_INCOMPATIBLE_VIDEO_PARAM:
					break;
				case MFX_ERR_REALLOC_SURFACE:
					break;
				default:
					printf("unknown status %d\n", sts);
					is_going = false;
					break;
			}
		}

        printf("Decoded %d frames\n", framenum);

		if (source)
			fclose(source);

		if (sink)
			fclose(sink);

		MFXVideoDECODE_Close(session);
		MFXClose(session);

		if (bitstream.Data)
			free(bitstream.Data);

		if (loader)
			MFXUnload(loader);

		return framenum;
	}

}

#endif