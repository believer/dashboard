let component = ReasonReact.statelessComponent("GitHubContainer");

module GetNotifications = [%graphql
  {|
  query gitHubNotifications {
    gitHubNotifications {
      date
      id
      subject {
        type_
        url
        title
      }
      repository {
        name
      }
    }
  }
|}
];

module GetNotificationsQuery = ReasonApollo.CreateQuery(GetNotifications);

module MarkAllAsRead = [%graphql
  {|
    mutation gitHubMarkAllAsRead {
      gitHubMarkAllAsRead
    }
|}
];

module MarkAllAsReadMutation = ReasonApollo.CreateMutation(MarkAllAsRead);

let make = _children => {
  ...component,

  render: _self =>
    <MarkAllAsReadMutation>
      ...{
           (markAllAsRead, _) =>
             <div className="w-100 w-50-l mr4 mb4">
               <GetNotificationsQuery pollInterval=GitHub.Config.interval>
                 ...{
                      ({result}) =>
                        <>
                          <Header
                            color="b--dark-blue"
                            hasItems={
                              switch (result) {
                              | Data(response) =>
                                Belt.Array.length(
                                  response##gitHubNotifications,
                                )
                                > 0
                              | _ => false
                              }
                            }
                            markAllAsRead={
                              _ =>
                                markAllAsRead(
                                  ~refetchQueries=[|"gitHubNotifications"|],
                                  (),
                                )
                                |> ignore
                            }
                            title="GitHub"
                          />
                          <Card>
                            {
                              GitHub.Config.hasConfig ?
                                switch (result) {
                                | Loading => <CardLoading />
                                | Error(error) =>
                                  <CardError errorMessage=error##message />
                                | Data(response) =>
                                  let notifications =
                                    response##gitHubNotifications;
                                  let groupedByDate =
                                    Utils.groupByDate(notifications);
                                  let hasNotifications =
                                    Belt.Array.length(notifications) > 0;

                                  GitHub.Config.setNumberOfNotifications(
                                    notifications,
                                  );

                                  DocumentTitle.updateTitleWithNotifications()
                                  |> ignore;

                                  Notify.sendGitHubNotification(
                                    notifications,
                                  );

                                  hasNotifications ?
                                    Js.Dict.values(groupedByDate)
                                    ->Belt.Array.mapWithIndex((index, list) => {
                                        let title = Js.Dict.keys(
                                                      groupedByDate,
                                                    )[index];

                                        <ListByDate
                                          index
                                          key=title
                                          title
                                          totalItems={
                                            Js.Dict.values(groupedByDate)
                                            ->Belt.Array.length
                                          }>
                                          {
                                            list->Belt.Array.mapWithIndex(
                                              (i, item) =>
                                              <GitHubNotification
                                                item
                                                isLast={
                                                  Belt.Array.length(list)
                                                  - 1 === i
                                                }
                                                key=item##id
                                              />
                                            )
                                            |> ReasonReact.array
                                          }
                                        </ListByDate>;
                                      })
                                    |> ReasonReact.array :
                                    <EmptyState />;
                                } :
                                <GitHubMissing />
                            }
                          </Card>
                        </>
                    }
               </GetNotificationsQuery>
             </div>
         }
    </MarkAllAsReadMutation>,
};
