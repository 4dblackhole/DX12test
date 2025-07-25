﻿//***************************************************************************************
// Init Direct3D.cpp by Frank Luna (C) 2015 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//***************************************************************************************

#include "framework.h"
#include "InitDirect3DApp.h"

using namespace DirectX;

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance)
    : D3DApp(hInstance)
{
}

InitDirect3DApp::~InitDirect3DApp()
{
}

bool InitDirect3DApp::Initialize()
{
    if (!__super::Initialize())
        return false;

    return true;
}

void InitDirect3DApp::OnResize()
{
    __super::OnResize();
}

void InitDirect3DApp::Update(const GameTimer& gt)
{
}

void InitDirect3DApp::Draw(const GameTimer& gt)
{
    // 커맨드 기록을 위한 메모리를 재활용 합니다.
    // 제출한 커맨드들이 GPU에서 모두 끝났을때 리셋할 수 있습니다.
    ThrowIfFailed(mDirectCmdListAlloc->Reset());

    // ExecuteCommandList를 통해 커맨드 큐에 제출한 다음에 커맨드 리스트를 리셋할 수 있습니다.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

    // 리소스의 상태를 렌더링을 할 수 있도록 변경합니다.
    const CD3DX12_RESOURCE_BARRIER resourceBarrier1
    { CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), 
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET) };
    mCommandList->ResourceBarrier(1, &resourceBarrier1);

    // 뷰포트와 시져를 설정한다. 커맨드 리스트가 리셋되고 나서 설정해야만 합니다.
    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    // 백 버퍼와 뎁스 버퍼를 클리어 합니다.
    const auto& dsv = DepthStencilView();
    const auto& currentBackBufferView = CurrentBackBufferView();
    mCommandList->ClearRenderTargetView(currentBackBufferView, Colors::AliceBlue, 0, nullptr);
    mCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    // 어디에 렌더링을 할지 설정합니다.
    mCommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &dsv);

    // 리소스의 상태를 출력할 수 있도록 변경합니다.
    const CD3DX12_RESOURCE_BARRIER resourceBarrier2
    { CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT) };
    mCommandList->ResourceBarrier(1, &resourceBarrier2);

    // 커맨드 기록을 종료합니다.
    ThrowIfFailed(mCommandList->Close());

    // 커맨드 리스트의 실행을 위해 큐에 제출합니다.
    ID3D12CommandList* cmdLists[] = {mCommandList.Get()};
    mCommandQueue->ExecuteCommandLists(1, cmdLists);

    // 백 버퍼와 프론트 버퍼를 교체합니다.
    ThrowIfFailed(mSwapChain->Present(0, 0));

    // 프레임의 커맨드들이 모두 처리될 때까지 기다립니다.
    // 이렇게 기다리는것은 비효율적이지만 예제를 간단하게 하기 위해서 기다립니다.
    // 나중에 기다리지 않고 렌더링을 하는법을 알아볼 것입니다.
    FlushCommandQueue();
}