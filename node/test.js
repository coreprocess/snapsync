const assert = require('assert');
const snapsync = require('./build/Release/snapsync.node');

// wrap callbacks in promises

function async_snap_create(dir, file) {
	return new Promise(function(resolve, reject) {
		snapsync.snap.create(dir, file, (error) => { if (error) { return reject(error) } else { return resolve() } });
	});
}

function async_snap_extract(file, dir) {
	return new Promise(function(resolve, reject) {
		snapsync.snap.extract(file, dir, (error) => { if (error) { return reject(error) } else { return resolve() } });
	});
}

function async_sync_signature(file, sig, bs, ss) {
	return new Promise(function(resolve, reject) {
		snapsync.sync.signature(file, sig, bs, ss, (error) => { if (error) { return reject(error) } else { return resolve() } });
	});
}

function async_sync_delta(sig, target, patch) {
	return new Promise(function(resolve, reject) {
		snapsync.sync.delta(sig, target, patch, (error) => { if (error) { return reject(error) } else { return resolve() } });
	});
}

function async_sync_patch(patch, base, target) {
	return new Promise(function(resolve, reject) {
		snapsync.sync.patch(patch, base, target, (error) => { if (error) { return reject(error) } else { return resolve() } });
	});
}

async function doAsyncTests() {
	await async_snap_create('targets', 'test-targets-async-v1').then().catch( error => assert.ifError(error) );
	await async_snap_extract('test-targets-async-v1', 'test-targets-async-remove-me').then().catch( error => assert.ifError(error) );
	await async_sync_signature('test-targets-async-v1', 'test-targets-async-v1.sig', 0, 8).then().catch( error => assert.ifError(error) );
	await async_sync_delta('test-targets-async-v1.sig', 'test-targets-async-v1', 'test-targets-async-v1v2.patch').then().catch( error => assert.ifError(error) );
	await async_sync_patch('test-targets-async-v1v2.patch', 'test-targets-async-v1', 'test-targets-async-v2').then().catch( error => assert.ifError(error) );
}

function doSyncTests() {
	assert.ifError(snapsync.snap.create('targets', 'test-targets-v1'));
	assert.ifError(snapsync.snap.extract('test-targets-v1', 'test-targets-remove-me'));
	assert.ifError(snapsync.sync.signature('test-targets-v1', 'test-targets-v1.sig', 0, 8));
	assert.ifError(snapsync.sync.delta('test-targets-v1.sig', 'test-targets-v1', 'test-targets-v1v2.patch'));
	assert.ifError(snapsync.sync.patch('test-targets-v1v2.patch', 'test-targets-v1', 'test-targets-v2'));
}

doSyncTests();
doAsyncTests();

setTimeout(function () {
    console.log('wait completed'); 
}, 1000);
