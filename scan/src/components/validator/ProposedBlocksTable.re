module Styles = {
  open Css;

  let tableWrapper = style([Media.mobile([padding2(~v=`px(16), ~h=`zero)])]);
  let icon = style([width(`px(80)), height(`px(80))]);
  let iconWrapper =
    style([
      width(`percent(100.)),
      display(`flex),
      flexDirection(`column),
      alignItems(`center),
    ]);
  let emptyContainer =
    style([
      height(`px(300)),
      display(`flex),
      justifyContent(`center),
      alignItems(`center),
      flexDirection(`column),
      backgroundColor(white),
    ]);
  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
};

//TODO: Will Remove After Doing on Validator index

module LoadingWithHeader = {
  [@react.component]
  let make = () => {
    <div className=Styles.tableWrapper>
      <THead.Grid>
        <Row.Grid alignItems=Row.Center>
          <Col.Grid col=Col.Two> <div /> </Col.Grid>
          <Col.Grid col=Col.Seven>
            <Text block=true value="Block Hash" weight=Text.Semibold color=Colors.gray7 />
          </Col.Grid>
          <Col.Grid col=Col.One>
            <Text block=true value="Txn" weight=Text.Semibold color=Colors.gray7 />
          </Col.Grid>
          <Col.Grid col=Col.Two>
            <Text
              block=true
              value="Timestamp"
              weight=Text.Semibold
              color=Colors.gray7
              align=Text.Right
            />
          </Col.Grid>
        </Row.Grid>
      </THead.Grid>
    </div>;
  };
};

let renderBody = (reserveIndex, blockSub: ApolloHooks.Subscription.variant(BlockSub.t)) => {
  <TBody.Grid
    key={
      switch (blockSub) {
      | Data({height}) => height |> ID.Block.toString
      | _ => reserveIndex |> string_of_int
      }
    }
    paddingH={`px(24)}>
    <Row.Grid alignItems=Row.Center minHeight={`px(30)}>
      <Col.Grid col=Col.Two>
        {switch (blockSub) {
         | Data({height}) => <TypeID.Block id=height />
         | _ => <LoadingCensorBar width=135 height=15 />
         }}
      </Col.Grid>
      <Col.Grid col=Col.Seven>
        {switch (blockSub) {
         | Data({hash}) =>
           <Text value={hash |> Hash.toHex(~upper=true)} block=true code=true ellipsis=true />

         | _ => <LoadingCensorBar width=522 height=15 />
         }}
      </Col.Grid>
      <Col.Grid col=Col.One>
        <div className={CssHelper.flexBox(~justify=`center, ())}>
          {switch (blockSub) {
           | Data({txn}) => <Text value={txn |> Format.iPretty} align=Text.Center />
           | _ => <LoadingCensorBar width=20 height=15 />
           }}
        </div>
      </Col.Grid>
      <Col.Grid col=Col.Two>
        <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
          {switch (blockSub) {
           | Data({timestamp}) =>
             <Timestamp.Grid
               time=timestamp
               size=Text.Md
               weight=Text.Regular
               textAlign=Text.Right
             />
           | _ => <LoadingCensorBar width=100 height=15 />
           }}
        </div>
      </Col.Grid>
    </Row.Grid>
  </TBody.Grid>;
};

let renderBodyMobile = (reserveIndex, blockSub: ApolloHooks.Subscription.variant(BlockSub.t)) => {
  switch (blockSub) {
  | Data({height, timestamp, txn, hash}) =>
    <MobileCard
      values=InfoMobileCard.[
        ("Block", Height(height)),
        ("Block Hash", TxHash(hash, Media.isSmallMobile() ? 170 : 200)),
        ("Txn", Count(txn)),
        ("Timestamp", Timestamp(timestamp)),
      ]
      key={height |> ID.Block.toString}
      idx={height |> ID.Block.toString}
    />
  | _ =>
    <MobileCard
      values=InfoMobileCard.[
        ("Block", Loading(Media.isSmallMobile() ? 170 : 200)),
        ("Block Hash", Loading(166)),
        ("Txn", Loading(20)),
        ("Timestamp", Loading(166)),
      ]
      key={reserveIndex |> string_of_int}
      idx={reserveIndex |> string_of_int}
    />
  };
};

[@react.component]
let make = (~consensusAddress) => {
  let (page, setPage) = React.useState(_ => 1);
  let pageSize = 10;

  let blocksSub =
    BlockSub.getListByConsensusAddress(~address=consensusAddress, ~pageSize, ~page, ());
  let blocksCountSub = BlockSub.countByConsensusAddress(~address=consensusAddress, ());

  let allSub = Sub.all2(blocksSub, blocksCountSub);

  let isMobile = Media.isMobile();
  <div className=Styles.tableWrapper>
    {isMobile
       ? <Row.Grid marginBottom=16>
           <Col.Grid>
             {switch (allSub) {
              | Data((_, blocksCount)) =>
                <div className={CssHelper.flexBox()}>
                  <Text
                    block=true
                    value={blocksCount |> string_of_int}
                    weight=Text.Semibold
                    color=Colors.gray7
                  />
                  <HSpacing size=Spacing.xs />
                  <Text block=true value="Blocks" weight=Text.Semibold color=Colors.gray7 />
                </div>
              | _ => <LoadingCensorBar width=100 height=15 />
              }}
           </Col.Grid>
         </Row.Grid>
       : <THead.Grid>
           <Row.Grid alignItems=Row.Center>
             <Col.Grid col=Col.Two>
               {switch (allSub) {
                | Data((_, blocksCount)) =>
                  <div className={CssHelper.flexBox()}>
                    <Text
                      block=true
                      value={blocksCount |> string_of_int}
                      weight=Text.Semibold
                      color=Colors.gray7
                    />
                    <HSpacing size=Spacing.xs />
                    <Text block=true value="Blocks" weight=Text.Semibold color=Colors.gray7 />
                  </div>
                | _ => <LoadingCensorBar width=100 height=15 />
                }}
             </Col.Grid>
             <Col.Grid col=Col.Seven>
               <Text block=true value="Block Hash" weight=Text.Semibold color=Colors.gray7 />
             </Col.Grid>
             <Col.Grid col=Col.One>
               <Text
                 block=true
                 value="Txn"
                 weight=Text.Semibold
                 color=Colors.gray7
                 align=Text.Center
               />
             </Col.Grid>
             <Col.Grid col=Col.Two>
               <Text
                 block=true
                 value="Timestamp"
                 weight=Text.Semibold
                 color=Colors.gray7
                 align=Text.Right
               />
             </Col.Grid>
           </Row.Grid>
         </THead.Grid>}
    {switch (allSub) {
     | Data((blocks, blockCount)) =>
       let pageCount = Page.getPageCount(blockCount, pageSize);
       <>
         {blockCount > 0
            ? blocks
              ->Belt_Array.mapWithIndex((i, e) =>
                  isMobile
                    ? renderBodyMobile(i, Sub.resolve(e)) : renderBody(i, Sub.resolve(e))
                )
              ->React.array
            : <div className=Styles.emptyContainer>
                <img src=Images.noAccount className=Styles.noDataImage />
                <Heading
                  size=Heading.H4
                  value="No Delegator"
                  align=Heading.Center
                  weight=Heading.Regular
                  color=Colors.bandBlue
                />
              </div>}
         {isMobile
            ? React.null
            : <Pagination
                currentPage=page
                pageCount
                onPageChange={newPage => setPage(_ => newPage)}
              />}
       </>;
     | _ =>
       Belt_Array.make(pageSize, ApolloHooks.Subscription.NoData)
       ->Belt_Array.mapWithIndex((i, noData) =>
           isMobile ? renderBodyMobile(i, noData) : renderBody(i, noData)
         )
       ->React.array
     }}
  </div>;
};
