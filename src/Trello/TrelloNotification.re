let component = ReasonReact.statelessComponent("TrelloNotification");

module MarkAsRead = [%graphql
  {|
    mutation trelloMarkAsRead($id: ID!) {
      trelloMarkAsRead(id: $id)
    }
|}
];

module MarkAsReadMutation = ReasonApollo.CreateMutation(MarkAsRead);

let trelloMessage = item => {
  let date = DateFns.format(item##date, "HH:mm");

  (
    switch (item##type_) {
    | "makeAdminOfOrganization" => date
    | "addedToOrganization" => date
    | _ =>
      (
        switch (item##data##board) {
        | Some(b) => " " ++ {js|•|js} ++ " " ++ b##name
        | None => ""
        }
      )
      ++ " "
      ++ (
        switch (item##creator) {
        | Some(c) => {js|•|js} ++ " " ++ c##fullName ++ " "
        | None => ""
        }
      )
      ++ {js|•|js}
      ++ " "
      ++ date
    }
  )
  |> Utils.str;
};

let make = (~isLast, ~item, _children) => {
  ...component,
  render: _self => {
    <MarkAsReadMutation>
      ...{(mutation, _) => {
        let markAsRead = MarkAsRead.make(~id=item##id, ());

        <div className={Cn.make(["flex", "mb3"->Cn.ifTrue(!isLast)])}>
          <div className="mr4">
            <TrelloIcon
              card=item##data##card
              listAfter=item##data##listAfter
              listBefore=item##data##listBefore
              text=item##data##text
            />
            <IconCheckCircle
              className="pointer mt2 green"
              onClick={_ =>
                mutation(
                  ~variables=markAsRead##variables,
                  ~refetchQueries=[|"trelloNotifications"|],
                  (),
                )
                |> ignore
              }
            />
          </div>
          <div className="f6">
            {switch (item##type_) {
             | "makeAdminOfOrganization" =>
               <div className="b mb-2 lh-copy">
                 "Made admin of organization"->Utils.str
               </div>
             | "addedToOrganization" =>
               <div className="b mb-2 lh-copy">
                 "Added to organization"->Utils.str
               </div>
             | _ =>
               switch (item##data##card) {
               | Some(card) =>
                 <div className="b mb2 lh-copy"> {card##name->Utils.str} </div>
               | None => ReasonReact.null
               }
             }}
            <TrelloText
              attachment=item##data##attachment
              listAfter=item##data##listAfter
              listBefore=item##data##listBefore
              text=item##data##text
            />
            <div className="mid-gray">
              {switch (item##data##card) {
               | Some(card) =>
                 <ExternalLink
                   href={"https://trello.com/c/" ++ card##shortLink}>
                   "Link"->Utils.str
                 </ExternalLink>
               | None => ReasonReact.null
               }}
              {trelloMessage(item)}
            </div>
          </div>
        </div>;
      }}
    </MarkAsReadMutation>;
  },
};
