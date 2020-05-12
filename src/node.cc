#if defined(BUILD_NODE_MODULE)

#include <nan.h>
#include "snap.hh"
#include "sync.hh"
#include <iostream>

using namespace snapsync;

class SnapCreateAsyncWorker : public Nan::AsyncWorker {
public:
	boost::filesystem::path directory;
	boost::filesystem::path image;

	SnapCreateAsyncWorker(boost::filesystem::path directory, boost::filesystem::path image, Nan::Callback *callback) : Nan::AsyncWorker(callback) {
		this->directory = directory;
		this->image = image;
	}

	void Execute() {
		try {
			snap::create(directory, image);
		}
		catch(const std::exception& e) {
			return this->SetErrorMessage(e.what());
		}
		catch(...) {
			return this->SetErrorMessage("an unknown error occured");
		}
	}

	void HandleOKCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
			Nan::Null(), // no error occured
		};
		Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 1, argv);
	}

	void HandleErrorCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
			Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
		};
		Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 1, argv);
	}
};

NAN_METHOD(snapCreate) {

	int nparams = info.Length();
	Nan::Callback* cb = NULL;

	if(nparams < 2 || nparams > 3) {
		return Nan::ThrowError(Nan::New("expected 2 to 3 arguments").ToLocalChecked());
	}

	if(!info[0]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 1 must be a string").ToLocalChecked());
	}

	boost::filesystem::path directory(*Nan::Utf8String(info[0]->ToString()));

	if(!info[1]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 2 must be a string").ToLocalChecked());
	}

	boost::filesystem::path image(*Nan::Utf8String(info[1]->ToString()));

	if(nparams == 3) {
		if(info[2]->IsFunction()) {
			cb = new Nan::Callback(info[2].As<v8::Function>());
		} else {
			return Nan::ThrowError(Nan::New("argument 3 must be a function").ToLocalChecked());
		}
	}

	if(cb) {	
		Nan::AsyncQueueWorker(new SnapCreateAsyncWorker(directory, image, cb));
	} else {
		try {
			snap::create(directory, image);
		}
		catch(const std::exception& e) {
			return Nan::ThrowError(Nan::New(e.what()).ToLocalChecked());
		}
		catch(...) {
			return Nan::ThrowError(Nan::New("an unknown error occured").ToLocalChecked());
		}
	}
}

class SnapExtractAsyncWorker : public Nan::AsyncWorker {
public:
	boost::filesystem::path image;
	boost::filesystem::path directory;

	SnapExtractAsyncWorker(boost::filesystem::path image, boost::filesystem::path directory, Nan::Callback *callback) : Nan::AsyncWorker(callback) {
		this->image = image;
		this->directory = directory;
	}

	void Execute() {
		try {
			snap::extract(image, directory);
		}
		catch(const std::exception& e) {
			return this->SetErrorMessage(e.what());
		}
		catch(...) {
			return this->SetErrorMessage("an unknown error occured");
		}
	}

	void HandleOKCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
			Nan::Null(), // no error occured
		};
		Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 1, argv);
	}

	void HandleErrorCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
			Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
		};
		Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 1, argv);
	}
};

NAN_METHOD(snapExtract) {

	int nparams = info.Length();
	Nan::Callback* cb = NULL;

	if(nparams < 2 || nparams > 3) {
		return Nan::ThrowError(Nan::New("expected 2 to 3 arguments").ToLocalChecked());
	}

	if(!info[0]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 1 must be a string").ToLocalChecked());
	}

	boost::filesystem::path image(*Nan::Utf8String(info[0]->ToString()));

	if(!info[1]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 2 must be a string").ToLocalChecked());
	}

	boost::filesystem::path directory(*Nan::Utf8String(info[1]->ToString()));

	if(nparams == 3) {
		if(info[2]->IsFunction()) {
			cb = new Nan::Callback(info[2].As<v8::Function>());
		} else {
			return Nan::ThrowError(Nan::New("argument 3 must be a function").ToLocalChecked());
		}
	}

	if(cb) {	
		Nan::AsyncQueueWorker(new SnapExtractAsyncWorker(image, directory, cb));
	} else {
		try {
			snap::extract(image, directory);
		}
		catch(const std::exception& e) {
			return Nan::ThrowError(Nan::New(e.what()).ToLocalChecked());
		}
		catch(...) {
			return Nan::ThrowError(Nan::New("an unknown error occured").ToLocalChecked());
		}
	}
}

class SyncSignatureAsyncWorker : public Nan::AsyncWorker {
public:
	boost::filesystem::path basePath;
	boost::filesystem::path signaturePath;
	size_t blockLength;
	size_t sumLength;

	SyncSignatureAsyncWorker(boost::filesystem::path basePath, boost::filesystem::path signaturePath, size_t blockLength, size_t sumLength, Nan::Callback *callback) : Nan::AsyncWorker(callback) {
		this->basePath = basePath;
		this->signaturePath = signaturePath;
		this->blockLength = blockLength;
		this->sumLength = sumLength;
	}

	void Execute() {
		try {
			sync::signature(basePath, signaturePath, blockLength, sumLength);
		}
		catch(const std::exception& e) {
			return this->SetErrorMessage(e.what());
		}
		catch(...) {
			return this->SetErrorMessage("an unknown error occured");
		}
	}

	void HandleOKCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
			Nan::Null(), // no error occured
		};
		Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 1, argv);
	}

	void HandleErrorCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
			Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
		};
		Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 1, argv);
	}
};

NAN_METHOD(syncSignature) {
	
	size_t blockLength = 0;
	size_t sumLength = 0;
	Nan::Callback* cb = NULL;
	int nparams = info.Length();

	if(nparams < 2 || nparams > 5) {
		return Nan::ThrowError(Nan::New("expected 2 to 5 arguments").ToLocalChecked());
	}

	if(!info[0]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 1 must be a string").ToLocalChecked());
	}

	boost::filesystem::path basePath(*Nan::Utf8String(info[0]->ToString()));

	if(!info[1]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 2 must be a string").ToLocalChecked());
	}

	boost::filesystem::path signaturePath(*Nan::Utf8String(info[1]->ToString()));

	if(nparams == 3) {
		if(info[2]->IsInt32()) {
			blockLength = Nan::To<uint32_t>(info[2]).FromMaybe(0);
		} else if(info[2]->IsFunction()) {
			cb = new Nan::Callback(info[2].As<v8::Function>());
		} else {
			return Nan::ThrowError(Nan::New("argument 3 must be an int or a function").ToLocalChecked());
		}
	} else if(nparams == 4) {
		if(info[2]->IsInt32()) {
			blockLength = Nan::To<uint32_t>(info[2]).FromMaybe(0);
		} else {
			return Nan::ThrowError(Nan::New("argument 3 must be an int").ToLocalChecked());
		}
		if(info[3]->IsInt32()) {
			sumLength = Nan::To<uint32_t>(info[3]).FromMaybe(0);
		} else if(info[3]->IsFunction()) {
			cb = new Nan::Callback(info[3].As<v8::Function>());
		} else {
			return Nan::ThrowError(Nan::New("argument 4 must be an int or a function").ToLocalChecked());
		}
	} else if(nparams == 5) {
		if(info[2]->IsInt32()) {
			blockLength = Nan::To<uint32_t>(info[2]).FromMaybe(0);
		} else {
			return Nan::ThrowError(Nan::New("argument 3 must be an int").ToLocalChecked());
		}
		if(info[3]->IsInt32()) {
			sumLength = Nan::To<uint32_t>(info[3]).FromMaybe(0);
		} else {
			return Nan::ThrowError(Nan::New("argument 4 must be an int").ToLocalChecked());
		}
		if(info[4]->IsFunction()) {
			cb = new Nan::Callback(info[4].As<v8::Function>());
		} else {
			return Nan::ThrowError(Nan::New("argument 5 must be a function").ToLocalChecked());
		}
	} else {
		return Nan::ThrowError(Nan::New("invalid parameters").ToLocalChecked()); // should never be reached
	}

	if(cb) {	
		Nan::AsyncQueueWorker(new SyncSignatureAsyncWorker(basePath, signaturePath, blockLength, sumLength, cb));
	} else {
		try {
			sync::signature(basePath, signaturePath, blockLength, sumLength);
		}
		catch(const std::exception& e) {
			return Nan::ThrowError(Nan::New(e.what()).ToLocalChecked());
		}
		catch(...) {
			return Nan::ThrowError(Nan::New("an unknown error occured").ToLocalChecked());
		}
	}
}

class SyncDeltaAsyncWorker : public Nan::AsyncWorker {
public:
	boost::filesystem::path signaturePath;
	boost::filesystem::path targetPath;
	boost::filesystem::path patchPath;

	SyncDeltaAsyncWorker(boost::filesystem::path signaturePath, boost::filesystem::path targetPath, boost::filesystem::path patchPath, Nan::Callback *callback) : Nan::AsyncWorker(callback) {
		this->signaturePath = signaturePath;
		this->targetPath = targetPath;
		this->patchPath = patchPath;
	}

	void Execute() {
		try {
			sync::delta(signaturePath, targetPath, patchPath);
		}
		catch(const std::exception& e) {
			return this->SetErrorMessage(e.what());
		}
		catch(...) {
			return this->SetErrorMessage("an unknown error occured");
		}
	}

	void HandleOKCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
			Nan::Null(), // no error occured
		};
		Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 1, argv);
	}

	void HandleErrorCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
			Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
		};
		Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 1, argv);
	}
};

NAN_METHOD(syncDelta) {

	int nparams = info.Length();
	Nan::Callback* cb = NULL;

	if(info.Length() < 3 || info.Length() > 4) {
		return Nan::ThrowError(Nan::New("expected 3 to 4 arguments").ToLocalChecked());
	}

	if(!info[0]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 1 must be a string").ToLocalChecked());
	}

	boost::filesystem::path signaturePath(*Nan::Utf8String(info[0]->ToString()));

	if(!info[1]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 2 must be a string").ToLocalChecked());
	}

	boost::filesystem::path targetPath(*Nan::Utf8String(info[1]->ToString()));

	if(!info[2]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 3 must be a string").ToLocalChecked());
	}

	boost::filesystem::path patchPath(*Nan::Utf8String(info[2]->ToString()));

	if(nparams == 4) {
		if(info[3]->IsFunction()) {
			cb = new Nan::Callback(info[3].As<v8::Function>());
		} else {
			return Nan::ThrowError(Nan::New("argument 4 must be a function").ToLocalChecked());
		}
	}
	
	if(cb) {	
		Nan::AsyncQueueWorker(new SyncDeltaAsyncWorker(signaturePath, targetPath, patchPath, cb));
	} else {
		try {
			sync::delta(signaturePath, targetPath, patchPath);
		}
		catch(const std::exception& e) {
			return Nan::ThrowError(Nan::New(e.what()).ToLocalChecked());
		}
		catch(...) {
			return Nan::ThrowError(Nan::New("an unknown error occured").ToLocalChecked());
		}
	}
}

class SyncPatchAsyncWorker : public Nan::AsyncWorker {
public:
	boost::filesystem::path patchPath;
	boost::filesystem::path basePath;
	boost::filesystem::path targetPath;

	SyncPatchAsyncWorker(boost::filesystem::path patchPath, boost::filesystem::path basePath, boost::filesystem::path targetPath, Nan::Callback *callback) : Nan::AsyncWorker(callback) {
		this->patchPath = patchPath;
		this->basePath = basePath;
		this->targetPath = targetPath;
	}

	void Execute() {
		try {
			sync::patch(patchPath, basePath, targetPath);
		}
		catch(const std::exception& e) {
			return this->SetErrorMessage(e.what());
		}
		catch(...) {
			return this->SetErrorMessage("an unknown error occured");
		}
	}

	void HandleOKCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
			Nan::Null(), // no error occured
		};
		Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 1, argv);
	}

	void HandleErrorCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
			Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
		};
		Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 1, argv);
	}
};

NAN_METHOD(syncPatch) {

	int nparams = info.Length();
	Nan::Callback* cb = NULL;

	if(nparams < 3 || nparams > 4) {
		return Nan::ThrowError(Nan::New("expected 3 to 4 arguments").ToLocalChecked());
	}

	if(!info[0]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 1 must be a string").ToLocalChecked());
	}

	boost::filesystem::path patchPath(*Nan::Utf8String(info[0]->ToString()));

	if(!info[1]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 2 must be a string").ToLocalChecked());
	}

	boost::filesystem::path basePath(*Nan::Utf8String(info[1]->ToString()));

	if(!info[2]->IsString()) {
		return Nan::ThrowError(Nan::New("argument 3 must be a string").ToLocalChecked());
	}

	boost::filesystem::path targetPath(*Nan::Utf8String(info[2]->ToString()));

	if(nparams == 4) {
		if(info[3]->IsFunction()) {
			cb = new Nan::Callback(info[3].As<v8::Function>());
		} else {
			return Nan::ThrowError(Nan::New("argument 4 must be a function").ToLocalChecked());
		}
	}

	if(cb) {	
		Nan::AsyncQueueWorker(new SyncPatchAsyncWorker(patchPath, basePath, targetPath, cb));
	} else {
		try {
			sync::patch(patchPath, basePath, targetPath);
		}
		catch(const std::exception& e) {
			return Nan::ThrowError(Nan::New(e.what()).ToLocalChecked());
		}
		catch(...) {
			return Nan::ThrowError(Nan::New("an unknown error occured").ToLocalChecked());
		}
	}
}

NAN_MODULE_INIT(init) {
	// snap methods
	auto snap = Nan::New<v8::Object>();

	Nan::Set(snap, Nan::New("create").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(snapCreate)->GetFunction());
	Nan::Set(snap, Nan::New("extract").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(snapExtract)->GetFunction());
	
	Nan::Set(target, Nan::New("snap").ToLocalChecked(), snap);

	// sync methods
	auto sync = Nan::New<v8::Object>();

	Nan::Set(sync, Nan::New("signature").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(syncSignature)->GetFunction());
	Nan::Set(sync, Nan::New("delta").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(syncDelta)->GetFunction());
	Nan::Set(sync, Nan::New("patch").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(syncPatch)->GetFunction());

	Nan::Set(target, Nan::New("sync").ToLocalChecked(), sync);
}

NODE_MODULE(snapsync, init)

#endif